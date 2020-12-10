#include "absyn.h"
#include "frontend.h"
#include "misc.h"
#include "symtab.h"

static parsed_type
parse_type(Type type)
{
    parsed_type retval;
    switch (type->kind) {
    case is_TCls:
    {
        retval.name = type->u.tcls_.ident_;
        retval.is_array = 0;
    } break;
    case is_TArr:
    {
        retval.name = type->u.tarr_.ident_;
        retval.is_array = 1;
    } break;
    }

    return retval;
}

// Assumes that the type exists
static processed_expr
get_default_expr(u32 type_id, void* node)
{
    processed_expr retval;
    retval.node = node;
    retval.type_id = type_id;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;

    switch (type_id) {
    case TYPEID_VOID:
    {
        // Probably will never happen, but just for safety:
        return retval;
    }
    case TYPEID_INT:
    case TYPEID_BOOL:
    {
        retval.u.cnst.numeric_val = 0;
        return retval;
    }
    case TYPEID_STRING:
    {
        retval.u.cnst.str_const_id = EMPTY_STRING_CONSTANT_ID;
        return retval;
    }
    default:
    {
        retval.node = node;
        retval.type_id = type_id;
        retval.kind = EET_COMPUTE;
        retval.is_lvalue = 1;

        return retval;
    }
    }
}

static processed_stmt
create_empty_statement(void)
{
    processed_stmt retval;
    retval.all_branches_return = 0;

    return retval;
}

static processed_expr
compute_binary_boolean_expr(mm v1, mm v2, binary_bool_op_t op, void* node)
{
    processed_expr retval;
    retval.type_id = TYPEID_BOOL;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;
    retval.node = node;
    switch (op) {
    case BBOP_AND: retval.u.cnst.numeric_val = !!(v1) && !!(v2); break;
    case BBOP_OR: retval.u.cnst.numeric_val = !!(v1) || !!(v2); break;
    }

    return retval;
}

static processed_expr
compute_binary_integer_expr(mm v1, mm v2, binary_int_op_t op, void* node)
{
    processed_expr retval;
    retval.type_id = TYPEID_INT;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;
    retval.node = node;
    switch (op) {
    case BIOP_ADD: retval.u.cnst.numeric_val = v1 + v2; break;
    case BIOP_SUB: retval.u.cnst.numeric_val = v1 - v2; break;
    case BIOP_MUL: retval.u.cnst.numeric_val = v1 * v2; break;
    case BIOP_DIV: retval.u.cnst.numeric_val = v1 / v2; break;
    case BIOP_MOD: retval.u.cnst.numeric_val = v1 % v2; break;
    case BIOP_LTH: retval.u.cnst.numeric_val = (v1 < v2); retval.type_id = TYPEID_BOOL; break;
    case BIOP_LE: retval.u.cnst.numeric_val = (v1 <= v2); retval.type_id = TYPEID_BOOL; break;
    case BIOP_GTH: retval.u.cnst.numeric_val = (v1 > v2); retval.type_id = TYPEID_BOOL; break;
    case BIOP_GE: retval.u.cnst.numeric_val = (v1 >= v2); retval.type_id = TYPEID_BOOL; break;
    }

    // Safe for booleans becasue they never overflow. In case of overflow, leave
    // 0 to have something reasonable for computations, we won't compile anyway
    if (UNLIKELY(overflows_32bit(retval.u.cnst.numeric_val)))
    {
        error(get_lnum(node), "Integer constant overflows");
        retval.u.cnst.numeric_val = 0;
    }

    return retval;
}

static processed_expr
compute_binary_string_expr(mm str1, mm str2, void* node)
{
    processed_expr retval;
    retval.type_id = TYPEID_STRING;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;
    retval.node = node;

    string_const s1 = g_str_consts[str1];
    string_const s2 = g_str_consts[str2];

    // This is allocated differently that strings hard-coded in code, and won't
    // be easy to free. But, since compiler works for short period of time, we
    // don't generally care about memory leaks here
    char* new_string = malloc((umm)(s1.len + s2.len + 1));
    new_string[0] = 0;
    strcat(new_string, s1.data);
    strcat(new_string, s2.data);

    string_const s;
    s.data = new_string;
    s.len = s1.len + s2.len;

    retval.u.cnst.str_const_id = array_size(g_str_consts);
    array_push(g_str_consts, s);

    return retval;
}

static i32 next_block_id = 1;
static i32
push_block(void)
{
    array_push(g_local_symbols, 0); // null is used to makr that the block ends
    return next_block_id++;
}

static void
pop_block(void)
{
    while (1)
    {
        mm size = array_size(g_local_symbols);
        (void)size;
        assert(size > 0);
        char* popped = array_pop(g_local_symbols);
        if (!popped) break;

        symbol_pop(popped);
    }
}

static inline void
push_var(d_var var, char* vname, void* node)
{
    symbol prev_decl = symbol_get(vname, node, 0);
    switch (prev_decl.type) {
    case S_NONE:
    {
        // No shadow, nothing to do
    } break;
    case S_VAR:
    {
        d_var v = g_vars[prev_decl.id];
        if (UNLIKELY(v.block_id) == var.block_id) // Redefinition, not just shadowing
        {
            error(get_lnum(node), "Variable \"%s\" redefined in the same block", vname);
            note(v.lnum, "Variable \"%s\" previously declared here", vname);

            return; // Don't push the sumbol
        }

        //
        // Shadowing an existing variable, which is OK. Can't write a warning,
        // because the compiler must outut either "ERROR" or "OK" to stdin, and
        // there might be some errors later.
        //

    } break;
    case S_FUN:
    {
        d_func f = g_funcs[prev_decl.id];
        error(get_lnum(node), "Variable \"%s\" cannot be named after a function", vname);
        note(f.lnum, "Function \"%s\" defined here", vname);

        return; // Don't push the sumbol
    } break;

    case S_TYPE:
    {
        if (prev_decl.id <= TYPEID_LAST_BUILTIN_TYPE)
        {
            error(get_lnum(node), "Variable \"%s\" cannot be named after a builtin type", vname);
        }
        else
        {
            d_type t = g_types[prev_decl.id];
            error(get_lnum(node), "Variable \"%s\" cannot be named after a class", vname);
            note(t.lnum, "Class \"%s\" defined here", vname);
        }

        return;
    } break;
    }

    mm var_id = array_size(g_vars);
    symbol sym;
    sym.type = S_VAR;
    sym.id = (i32)var_id;
    symbol_push(vname, sym);

    array_push(g_vars, var);
    array_push(g_local_symbols, vname);
}

static processed_expr
compute_binary_integer_or_boolean_equality_expr(mm v1, mm v2, binary_eq_op_t op, void* node)
{
    processed_expr retval;
    retval.type_id = TYPEID_BOOL;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;
    retval.node = node;
    retval.u.cnst.numeric_val = !((op == BEOP_EQ) ^ (v1 == v2));

    return retval;
}

static processed_expr
compute_binary_string_equality_expr(mm str1, mm str2, binary_eq_op_t op, void* node)
{
    b32 compre = strcmp(g_str_consts[str1].data, g_str_consts[str2].data) == 0;
    processed_expr retval;
    retval.type_id = TYPEID_BOOL;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;
    retval.node = node;
    retval.u.cnst.numeric_val = !((op == BEOP_EQ) ^ compre);

    return retval;
}

static void
enforce_type(processed_expr* e, u32 type_id)
{
    if (UNLIKELY(e->type_id != type_id))
    {
        d_type t_expected = g_types[TYPEID_UNMASK(type_id)];
        d_type t_got = g_types[TYPEID_UNMASK(e->type_id)];

        error(get_lnum(e->node),
              "Expected type \"%s\" but got incompatible type \"%s\"",
              t_expected.name, t_got.name);

        // Default to the desired type
        e->type_id = type_id;
        e->kind = EET_COMPUTE;
    }
}

static void
process_params(ListExpr arg_exprs, d_func* fun, void* node)
{
    // 1st param "self" for local func. So for local functions we start
    // evaluating params from the second arg. Error messages needs to be
    // adjusted to this hack!
    i32 n_given_args = 0 + !!fun->is_local;

    struct { ListExpr listexpr_; } expr_list;
    expr_list.listexpr_ = arg_exprs;
    LIST_FOREACH(it, expr_list, listexpr_)
    {
        if (LIKELY(n_given_args < fun->num_args)) // if not too many args given
        {
            Expr argexpr = it->expr_;
            processed_expr argexpr_e = process_expr(argexpr);
            u32 expected_type_id = fun->args[n_given_args].type_id;

            if (argexpr_e.type_id != expected_type_id) // TODO(ex): Handle inheritance
            {
                d_type t_expected = g_types[TYPEID_UNMASK(expected_type_id)];
                d_type t_provided = g_types[TYPEID_UNMASK(argexpr_e.type_id)];

                error(get_lnum(argexpr), "Function \"%s\" expects argument %d to have a type \"%s\"",
                      fun->name, n_given_args + 1 - !!(fun->is_local), t_expected.name);

                note(get_lnum(argexpr), "Given expression of type \"%s\", which is not assignable to type \"%s\"",
                     t_provided.name, t_expected.name);
            }
        }

        n_given_args++;
    }

    if (n_given_args != fun->num_args)
    {
        error(get_lnum(node),
              "Function \"%s\" takes %d arguments, but %d were provided",
              fun->name,
              fun->num_args - !!fun->is_local,
              n_given_args - !!fun->is_local);
    }
}

static processed_expr
process_expr(Expr e)
{
    processed_expr retval;
    Expr binarg1; // To be set for binary expressions
    Expr binarg2;
    binary_int_op_t binintop = BIOP_ADD;
    binary_bool_op_t binboolop = BBOP_AND;

    retval.node = e;
    retval.is_lvalue = 0;
    switch (e->kind) {
    case is_ELitTrue:
    case is_ELitFalse:
    {
        retval.type_id = TYPEID_BOOL;
        retval.kind = EET_CONSTANT;
        retval.is_lvalue = 0;
        retval.u.cnst.numeric_val = (e->kind == is_ELitTrue); // true/false
        return retval;
    }

    case is_ELitInt:
    {
        // Looks like BNFC (or bison (or flex?)) leaves here -1 if the number
        // was too large. Print a reasonable error, and treat this value as 0
        if (e->u.elitint_.integer_ < 0)
        {
            error(get_lnum(e), "Integer constant is too large");
            e->u.elitint_.integer_ = 0;
        }

        retval.type_id = TYPEID_INT;
        retval.kind = EET_CONSTANT;
        retval.is_lvalue = 0;
        retval.u.cnst.numeric_val = (i64)e->u.elitint_.integer_;
        return retval;
    }

    case is_ELitStr:
    {
        char* data = e->u.elitstr_.string_;
        string_const c = { .data = data, .len = (mm)strlen(data) };
        mm idx = array_size(g_str_consts);
        array_push(g_str_consts, c);

        retval.type_id = TYPEID_STRING;
        retval.kind = EET_CONSTANT;
        retval.is_lvalue = 0;
        retval.u.cnst.str_const_id = idx;
        return retval;
    }

    case is_Not:
    {
        processed_expr e1 = process_expr(e->u.not_.expr_);
        enforce_type(&e1, TYPEID_BOOL);

        if (UNLIKELY(e1.kind == EET_CONSTANT))
        {
            assert(!e1.is_lvalue);
            e1.u.cnst.numeric_val = !e1.u.cnst.numeric_val;
            return e1;
        }

        retval.type_id = TYPEID_BOOL;
        retval.kind = EET_COMPUTE;
        retval.is_lvalue = 0;
        return retval; // TODO(ir)
    }

    case is_Neg:
    {
        processed_expr e1 = process_expr(e->u.not_.expr_);
        enforce_type(&e1, TYPEID_INT);

        if (UNLIKELY(e1.kind == EET_CONSTANT))
        {
            assert(!e1.is_lvalue);
            e1.u.cnst.numeric_val = -e1.u.cnst.numeric_val;

            // In case when we have -(INT_MIN):
            if (UNLIKELY(overflows_32bit(e1.u.cnst.numeric_val)))
            {
                error(get_lnum(e1.node), "Integer constant overflows");
                retval.u.cnst.numeric_val = 0;
            }

            return e1;
        }

        retval.type_id = TYPEID_INT;
        retval.kind = EET_COMPUTE;
        retval.is_lvalue = 0;
        return retval; // TODO(ir)
    }

    case is_EMul:
    {
        binarg1 = e->u.emul_.expr_1;
        binarg2 = e->u.emul_.expr_2;
        switch (e->u.emul_.mulop_->kind) {
        case is_Times: binintop = BIOP_MUL; break;
        case is_Div: binintop = BIOP_DIV; break;
        case is_Mod: binintop = BIOP_MOD; break;
        }

        goto binary_integer_expr;
    }

    case is_EAdd:
    {
        binarg1 = e->u.eadd_.expr_1;
        binarg2 = e->u.eadd_.expr_2;
        switch (e->u.eadd_.addop_->kind) {
        case is_Plus: binintop = BIOP_ADD; break;
        case is_Minus: binintop = BIOP_SUB; break;
        }

        goto binary_integer_expr;
    }

    case is_ERel: // "==" is different expr (EEq), this one is for ints only
    {
        binarg1 = e->u.erel_.expr_1;
        binarg2 = e->u.erel_.expr_2;
        switch (e->u.erel_.relop_->kind) {
        case is_LTH: binintop = BIOP_LTH; break;
        case is_GTH: binintop = BIOP_GTH; break;
        case is_LE: binintop = BIOP_LE; break;
        case is_GE: binintop = BIOP_GE; break;
        }

        goto binary_integer_expr;
    }

    case is_EAnd:
    {
        binarg1 = e->u.eand_.expr_1;
        binarg2 = e->u.eand_.expr_2;
        binboolop = BBOP_AND;

        goto binary_boolean_expr;
    }

    case is_EOr:
    {
        binarg1 = e->u.eor_.expr_1;
        binarg2 = e->u.eor_.expr_2;
        binboolop = BBOP_OR;

        goto binary_boolean_expr;
    }

    binary_boolean_expr:
    {
        processed_expr e1 = process_expr(binarg1);
        processed_expr e2 = process_expr(binarg2);
        enforce_type(&e1, TYPEID_BOOL);
        enforce_type(&e2, TYPEID_BOOL);
        if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
        {
            assert(!e1.is_lvalue);
            assert(!e2.is_lvalue);

            mm v1 = e1.u.cnst.numeric_val;
            mm v2 = e2.u.cnst.numeric_val;
            return compute_binary_boolean_expr(v1, v2, binboolop, e1.node);
        }

        retval.type_id = TYPEID_BOOL;
        retval.kind = EET_COMPUTE;
        retval.is_lvalue = 0;
        return retval; // TODO(ir)
    }

    binary_integer_expr:
    {
        processed_expr e1 = process_expr(binarg1);
        processed_expr e2 = process_expr(binarg2);

        // This is the only case when an operator other that == and != is
        // overloaded. IMO string concatenation should have separate operator
        // (like '@'). Since this is a unique exception, we hack around it:
        if (e1.type_id == TYPEID_STRING && binintop == BIOP_ADD)
        {
            enforce_type(&e2, TYPEID_STRING);
            if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
            {
                assert(!e1.is_lvalue);
                assert(!e2.is_lvalue);

                mm str1 = e1.u.cnst.str_const_id;
                mm str2 = e2.u.cnst.str_const_id;
                return compute_binary_string_expr(str1, str2, e1.node);
            }

            retval.type_id = TYPEID_STRING;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 0;
            return retval; // TODO(ir)
        }

        enforce_type(&e1, TYPEID_INT);
        enforce_type(&e2, TYPEID_INT);
        if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
        {
            assert(!e1.is_lvalue);
            assert(!e2.is_lvalue);

            mm v1 = e1.u.cnst.numeric_val;
            mm v2 = e2.u.cnst.numeric_val;
            return compute_binary_integer_expr(v1, v2, binintop, e1.node);
        }

        if (binintop == BIOP_LTH || binintop == BIOP_LE
            || binintop == BIOP_GTH || binintop == BIOP_GE)
        {
            retval.type_id = TYPEID_BOOL;
        }
        else
        {
            retval.type_id = TYPEID_INT;
        }

        retval.kind = EET_COMPUTE;
        retval.is_lvalue = 0;
        return retval; // TODO(ir)
    }

    case is_EEq:
    {
        processed_expr e1 = process_expr(e->u.eeq_.expr_1);
        processed_expr e2 = process_expr(e->u.eeq_.expr_2);
        binary_eq_op_t bineqop = BEOP_EQ;
        switch (e->u.eeq_.eqop_->kind) {
        case is_NE: bineqop = BEOP_NEQ; break;
        case is_EQU: bineqop = BEOP_EQ; break;
        }

        switch (e1.type_id) {
        case TYPEID_INT:
        {
            if (e2.type_id != TYPEID_INT)
                goto type_missmatch;

            if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
                goto compute_contant_equality_int_or_bool;

            retval.type_id = TYPEID_BOOL;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 0;
            return retval; // TODO(ir)
        }

        case TYPEID_BOOL:
        {
            if (e2.type_id != TYPEID_BOOL)
                goto type_missmatch;

            if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
                goto compute_contant_equality_int_or_bool;

            retval.type_id = TYPEID_BOOL;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 0;
            return retval; // TODO(ir)
        }

        case TYPEID_STRING:
        {
            if (e2.type_id != TYPEID_STRING)
                goto type_missmatch;

            if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
                goto compute_contant_equality_string;

            retval.type_id = TYPEID_BOOL;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 0;
            return retval; // TODO(ir)
        }

        compute_contant_equality_int_or_bool:
        {
            assert(!e1.is_lvalue);
            assert(!e2.is_lvalue);

            mm v1 = e1.u.cnst.numeric_val;
            mm v2 = e2.u.cnst.numeric_val;
            return compute_binary_integer_or_boolean_equality_expr(v1, v2, bineqop, e1.node);
        }

        compute_contant_equality_string:
        {
            assert(!e1.is_lvalue);
            assert(!e2.is_lvalue);

            mm str1 = e1.u.cnst.str_const_id;
            mm str2 = e2.u.cnst.str_const_id;
            return compute_binary_string_equality_expr(str1, str2, bineqop, e1.node);
        }

        type_missmatch:
        {
            d_type lhs_t = g_types[TYPEID_UNMASK(e1.type_id)];
            d_type rhs_t = g_types[TYPEID_UNMASK(e2.type_id)];

            error(get_lnum(e1.node), "Comparing expressions with incompatible types");
            note(get_lnum(e1.node), "Left hand side evalutes to \"%s\"", lhs_t.name);
            note(get_lnum(e2.node), "Right hand side evalutes to \"%s\"", rhs_t.name);

            retval.type_id = TYPEID_BOOL;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 0;
            return retval; // TODO(ir)
        }

        //
        // Latte is unspecified when it comes to comparing the reference type
        // other than strings, so we asssume that this is basically the equality
        // of references (so that equality against null is meaningful).
        //
        default:
        {
            // TODO(ex): Make sure equality works when type A inherits from type B
            if (e1.type_id != e2.type_id)
                goto type_missmatch;

            retval.type_id = TYPEID_BOOL;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 0;
            return retval; // TODO(ir)
        }
        }
    }

    case is_EVar:
    {
        Ident var_name = e->u.evar_.ident_;
        u32 var_id = symbol_resolve_var(var_name, e);
        if (UNLIKELY(var_id == VARID_NOTFOUND))
        {
            note(get_lnum(e), "Assuming type \"int\"");
            retval.type_id = TYPEID_INT;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 1;
            return retval; // TODO(ir)
        }
        else
        {
            d_var var = g_vars[var_id];
            retval.type_id = var.type_id;
            retval.kind = EET_COMPUTE;
            retval.is_lvalue = 1;
            return retval; // TODO(ir)
        }
    }

    case is_EApp:
    {
        u32 func_id = symbol_resolve_func(e->u.eapp_.ident_, e);
        if (func_id == FUNCID_NOTFOUND)
        {
            // Can't do anything reasonable, so just asume that type is int
            note(get_lnum(e), "Assuming return type \"int\"");
            retval.is_lvalue = 0;
            retval.type_id = TYPEID_INT;
            retval.kind = EET_COMPUTE;
            return retval;
        }

        d_func* f = g_funcs + func_id;
        process_params(e->u.eapp_.listexpr_, f, e);

        retval.is_lvalue = 0; // Like in Java, functions always return rvalues
        retval.type_id = f->ret_type_id;
        retval.kind = EET_COMPUTE;
        return retval;
    }

    case is_ENew:
    {
        char* type_name = e->u.enew_.ident_;
        u32 type_id = symbol_resolve_type(type_name, 0, e);
        assert(type_id == TYPEID_NOTFOUND || !(type_id & TYPEID_FLAG_ARRAY));
        switch (type_id) {
        case TYPEID_NOTFOUND:
        {
            type_id = TYPEID_INT; // Default to int in order to avoid errors
        } break;
        case TYPEID_VOID:
        case TYPEID_INT:
        case TYPEID_BOOL:
        case TYPEID_STRING:
        {
            error(get_lnum(e), "Primitive types cannot be heap-allocated with new");
            type_id = TYPEID_INT;
        } break;
        default:
        {
        } break;
        }

        retval.type_id = type_id;
        retval.kind = EET_COMPUTE;
        return retval;
    }

    case is_ENewArr:
    {
        char* type_name = e->u.enewarr_.ident_;
        Expr esize = e->u.enewarr_.expr_;
        u32 type_id = symbol_resolve_type(type_name, 0, e);
        assert(type_id == TYPEID_NOTFOUND || !(type_id & TYPEID_FLAG_ARRAY));
        switch (type_id) {
        case TYPEID_NOTFOUND:
        {
            type_id = TYPEID_INT; // Default to int in order to avoid errors
        } break;
        case TYPEID_VOID:
        {
            error(get_lnum(e), "\"void\" type cannot be used in an array");
            type_id = TYPEID_INT;
        } break;
        default:
        {
        } break;
        }
        type_id |= TYPEID_FLAG_ARRAY; // The resulting type of the expr is array

        processed_expr e_esize = process_expr(esize);
        enforce_type(&e_esize, TYPEID_INT);

        retval.type_id = type_id;
        retval.kind = EET_COMPUTE;
        return retval;
    }

    case is_EArrApp:
    {
        processed_expr e_array = process_expr(e->u.earrapp_.expr_1);
        if (!(e_array.type_id & TYPEID_FLAG_ARRAY))
        {
            error(get_lnum(e->u.earrapp_.expr_1),
                  "Non-array cannot be a left-hand-side of the array subscript");
        }

        processed_expr e_idx = process_expr(e->u.earrapp_.expr_2);
        enforce_type(&e_idx, TYPEID_INT);

        // If given array is lvalue then it's subscript also is
        retval.is_lvalue = e_array.is_lvalue;
        retval.type_id = TYPEID_UNMASK(e_array.type_id);
        retval.kind = EET_COMPUTE;
        return retval;
    }

    case is_EClMem:
    {
        processed_expr e_struct = process_expr(e->u.eclmem_.expr_);
        if (e_struct.type_id & TYPEID_FLAG_ARRAY)
        {
            // In case of array, only .length is possible
            if (UNLIKELY(strcmp(e->u.eclmem_.ident_, "length") != 0))
            {
                error(get_lnum(e->u.eclmem_.expr_),
                      "Member field \"%s\" does not exists on the \"array\" type",
                      e->u.eclmem_.ident_);
            }

            retval.is_lvalue = 0;
            retval.type_id = TYPEID_INT;
            retval.kind = EET_COMPUTE;
            return retval;
        }
        else if (e_struct.type_id <= TYPEID_LAST_BUILTIN_TYPE)
        {
            d_type t_provided = g_types[TYPEID_UNMASK(e_struct.type_id)];
            error(get_lnum(e->u.eclmem_.expr_),
                  "Left-hand-side of . expression is not a struct nor array");
            note(get_lnum(e->u.eclmem_.expr_),
                 "Expression type evaluates to \"%s\"", t_provided.name);

            retval.type_id = TYPEID_INT;
            retval.kind = EET_COMPUTE;
            return retval;
        }
        else
        {
            d_type cltype = g_types[TYPEID_UNMASK(e_struct.type_id)];
            mm n_members = array_size(cltype.members);
            mm idx = 0;
            for (; idx < n_members; ++idx)
                if (strcmp(cltype.members[idx].name, e->u.eclmem_.ident_) == 0)
                {
                    retval.type_id = cltype.members[idx].type_id;
                    break;
                }

            if (UNLIKELY(idx == n_members))
            {
                error(get_lnum(e->u.eclmem_.expr_),
                      "Class \"%s\" does not have a member named \"%s\"",
                      cltype.name, e->u.eclmem_.ident_);
                note(get_lnum(e->u.eclmem_.expr_), "Assuming type \"int\"");

                retval.type_id = TYPEID_INT;
            }

            retval.is_lvalue = e_struct.is_lvalue;
            retval.kind = EET_COMPUTE;
            return retval;
        }

        NOTREACHED;
    }

    case is_EClApp:
    {
        processed_expr e_struct = process_expr(e->u.eclapp_.expr_);
        d_type cltype = g_types[e_struct.type_id];
        char* fn_name = e->u.eclapp_.ident_;

        if (cltype.is_primitive)
        {
            error(get_lnum(e->u.eclapp_.expr_), "Builtin types does not have a member functions");
            note(get_lnum(e->u.eclapp_.expr_), "Expresion evaluated to type \"%s\"", cltype.name);
            note(get_lnum(e->u.eclapp_.expr_), "Assuming return type \"int\"");
            retval.type_id = TYPEID_INT;
        }
        else
        {
            mm n_member_funcs = array_size(cltype.member_funcs);
            mm idx = 0;
            for (; idx < n_member_funcs; ++idx)
                if (strcmp(cltype.member_funcs[idx].name, fn_name) == 0)
                {
                    retval.type_id = cltype.member_funcs[idx].ret_type_id;
                    break;
                }

            if (UNLIKELY(idx == n_member_funcs))
            {
                error(get_lnum(e->u.eclapp_.expr_), "Function \"%s\" is not a member of class \"%s\"",
                      fn_name, cltype.name);
                note(cltype.lnum, "Class \"%s\" defined here", cltype.name);
                note(get_lnum(e->u.eclapp_.expr_), "Assuming type \"int\"");

                retval.type_id = TYPEID_INT;
            }
            else
            {
                d_func* f = cltype.member_funcs + idx;
                process_params(e->u.eclapp_.listexpr_, f, e);
            }
        }

        retval.is_lvalue = 0; // Like in Java, functions always return rvalues
        retval.kind = EET_COMPUTE;
        return retval;
    }

    case is_ENull:
    {
        retval.type_id = TYPEID_NULL;
        retval.kind = EET_CONSTANT;
        retval.is_lvalue = 0;
        retval.u.cnst.numeric_val = 0;
        return retval;
    }

    case is_ECast:
    {
        char* type_name = e->u.ecast_.ident_;
        u32 type_id = symbol_resolve_type(type_name, 0, e);
        assert(type_id == TYPEID_NOTFOUND || !(type_id & TYPEID_FLAG_ARRAY));
        switch (type_id) {
        case TYPEID_NOTFOUND:
        {
            note(get_lnum(e), "Assuming type \"int\"");
            type_id = TYPEID_INT;
        } break;
        case TYPEID_VOID:
        case TYPEID_INT:
        case TYPEID_BOOL:
        case TYPEID_STRING:
        case TYPEID_NULL:
        {
            error(get_lnum(e), "Casting to a builtin type is not allowed");

            // Return the desired type anyway - maybe we'll have less errors
            retval.type_id = type_id;
            retval.kind = EET_COMPUTE;
            return retval;
        } break;
        default:
        {
        } break;
        }

        processed_expr expr_to_cast = process_expr(e->u.ecast_.expr_);
        switch (expr_to_cast.type_id) {
        case TYPEID_NULL: // Null is always castable to desired type
        {
            retval.type_id = type_id;
            retval.kind = EET_COMPUTE;
            return retval;
        }
        default:
        {
        } break;
        }

        // Right now all types are incompatible.
        // TODO(ex): Adjust when inhericance is implemented
        if (expr_to_cast.type_id != type_id)
        {
            d_type* src_type = g_types + TYPEID_UNMASK(expr_to_cast.type_id);
            d_type* dest_type = g_types + TYPEID_UNMASK(type_id);
            error(get_lnum(e),
                  "Casting expression of type \"%s%s\" to incompatible type \"%s%s\"",
                  src_type->name,
                  expr_to_cast.type_id & TYPEID_FLAG_ARRAY ? "[]" : "",
                  dest_type->name,
                  type_id & TYPEID_FLAG_ARRAY ? "[]" : "");

            // In case of an error return desired type to avoid further errors
        }

        retval.type_id = type_id;
        retval.kind = EET_COMPUTE;
        return retval;
    }
    }

    NOTREACHED;
}

static processed_stmt
process_stmt(Stmt s, u32 return_type, i32 cur_block_id)
{
    processed_stmt retval;
    switch (s->kind) {
    case is_Empty:
    {
        return create_empty_statement();
    }

    case is_BStmt:
    {
        Block b = s->u.bstmt_.block_;
        assert(b->kind == is_Blk);

        b32 all_branches_return = 0;
        i32 new_block_id = push_block();
        LIST_FOREACH(it, b->u.blk_, liststmt_)
        {
            processed_stmt ev_s = process_stmt(it->stmt_, return_type, new_block_id);
            all_branches_return |= ev_s.all_branches_return;
        }
        pop_block();

        retval.all_branches_return = all_branches_return;
        return retval;
    }

    case is_SExp:
    {
        processed_expr ee = process_expr(s->u.sexp_.expr_);
        (void)ee;

        retval.all_branches_return = 0;
        return retval; // TODO(ir)
    }

    case is_Decl:
    {
        Type var_type = s->u.decl_.type_;
        parsed_type type = parse_type(s->u.decl_.type_);
        u32 type_id = symbol_resolve_type(type.name, type.is_array, var_type);

        switch (type_id) {
        case TYPEID_NOTFOUND:
        {
            note(get_lnum(var_type), "Assuming type \"int\"");
            type_id = TYPEID_INT;
        } break;
        case TYPEID_VOID:
        case TYPEID_VOID | TYPEID_FLAG_ARRAY:
        {
            error(get_lnum(var_type), "Cannot declare a variable of type \"void%s\"",
                  type_id & TYPEID_FLAG_ARRAY ? "[]" : "");

            note(get_lnum(var_type), "Assuming type \"int\"");
            type_id = TYPEID_INT;
        } break;
        default:
        {
        } break;
        }

        LIST_FOREACH(it, s->u.decl_, listitem_)
        {
            Item i = it->item_;
            char* vname = 0;
            processed_expr vval;
            switch (i->kind) {
            case is_NoInit:
            {
                vname = i->u.noinit_.ident_;
                vval = get_default_expr(type_id, i);
            } break;
            case is_Init:
            {
                vname = i->u.init_.ident_;
                vval = process_expr(i->u.init_.expr_);

                if (type_id != vval.type_id) // TODO(ex): Handle inheritance
                {
                    d_type expected_t = g_types[TYPEID_UNMASK(type_id)];
                    d_type got_t = g_types[TYPEID_UNMASK(vval.type_id)];
                    error(get_lnum(i),
                          "Expression of type \"%s%s\" is initialized with incompatible type \"%s%s\"",
                          expected_t.name, type_id & TYPEID_FLAG_ARRAY ? "[]" : "",
                          got_t.name, vval.type_id & TYPEID_FLAG_ARRAY ? "[]" : "");

                    // Clarify why null is not implicitely assignable to a reference type:
                    if (vval.type_id == TYPEID_NULL && type_id > TYPEID_LAST_BUILTIN_TYPE)
                    {
                        note(get_lnum(i),
                             "\"null\" is not implicitly assingable to a reference type. Use: \"(%s)null\"",
                             expected_t.name);
                        note(get_lnum(i),
                             "This requirement is forced by the Author of the assignment. Sorry");
                    }
                }
            } break;
            }

            assert(vname);

            // Regardless of whether or not inicializing errored declare the
            // variable with a type defined on the left, to avoid error cascade.
            d_var var;
            var.lnum = get_lnum(i);
            var.type_id = type_id;
            var.block_id = cur_block_id;
            push_var(var, vname, i);
        }

        retval.all_branches_return = 0;
        return retval;
    }

    case is_Ass:
    {
        processed_expr e1 = process_expr(s->u.ass_.expr_1);
        processed_expr e2 = process_expr(s->u.ass_.expr_2);
        if (UNLIKELY(!e1.is_lvalue))
        {
            error(get_lnum(e1.node), "Left side of assingment must be an lvalue");
        }
        else if (UNLIKELY(e1.type_id != e2.type_id)) // TODO(ex): Handle inheritance
        {
            d_type expected_t = g_types[TYPEID_UNMASK(e1.type_id)];
            d_type got_t = g_types[TYPEID_UNMASK(e2.type_id)];
            error(get_lnum(e2.node),
                  "Variable of type \"%s\" is assigned with incompatible type \"%s\"",
                  expected_t.name, got_t.name);

            // Clarify why null is not implicitely assignable to a reference type:
            if (e2.type_id == TYPEID_NULL && e1.type_id > TYPEID_LAST_BUILTIN_TYPE)
            {
                note(get_lnum(e2.node),
                     "\"null\" is not implicitly assingable to a reference type. Use: \"(%s)null\"",
                     expected_t.name);
                note(get_lnum(e2.node),
                     "This requirement is forced by the Author of the assignment. Sorry");
            }
        }

        retval.all_branches_return = 0;
        return retval;
    }

    case is_Ret:
    case is_VRet:
    {
        processed_expr e;
        if (s->kind == is_VRet)
        {
            e.node = s;
            e.type_id = TYPEID_VOID;
            e.kind = EET_CONSTANT;
            e.is_lvalue = 0;
        }
        else
        {
            e = process_expr(s->u.ret_.expr_);
        }

        if (UNLIKELY(e.type_id != return_type)) // TODO(ex): Handle inheritance
        {
            d_type expected_t = g_types[TYPEID_UNMASK(return_type)];
            d_type got_t = g_types[TYPEID_UNMASK(e.type_id)];
            error(get_lnum(e.node),
                  "Returning value of type \"%s\" incompatible with type \"%s\"",
                  got_t.name, expected_t.name);

            if (TYPEID_UNMASK(e.type_id) > TYPEID_LAST_BUILTIN_TYPE)
            {
                note(got_t.lnum, "Type \"%s\" defined here", got_t.name);
            }
        }

        retval.all_branches_return = 1;
        return retval;
    }

    case is_Cond:
    case is_CondElse:
    {
        processed_expr condex;
        processed_stmt s1;
        processed_stmt s2;
        if (s->kind == is_CondElse)
        {
            condex = process_expr(s->u.condelse_.expr_);
            i32 blk1 = push_block();
            s1 = process_stmt(s->u.condelse_.stmt_1, return_type, blk1);
            pop_block();
            i32 blk2 = push_block();
            s2 = process_stmt(s->u.condelse_.stmt_2, return_type, blk2);
            pop_block();
        }
        else
        {
            condex = process_expr(s->u.condelse_.expr_);
            i32 blk = push_block();
            s1 = process_stmt(s->u.condelse_.stmt_1, return_type, blk);
            pop_block();
            s2 = create_empty_statement();
        }

        retval.all_branches_return =
            ((condex.kind == EET_CONSTANT && condex.u.cnst.numeric_val == 1 && s1.all_branches_return)
             || (condex.kind == EET_CONSTANT && condex.u.cnst.numeric_val == 0 && s2.all_branches_return)
             || (s1.all_branches_return && s2.all_branches_return));

        return retval;
    }

    case is_Incr:
    case is_Decr:
    {
        Expr e = s->kind == is_Incr ? s->u.incr_.expr_ : s->u.decr_.expr_;
        processed_expr processed_e = process_expr(e);
        if (processed_e.type_id != TYPEID_INT || !processed_e.is_lvalue)
        {
            error(get_lnum(e), "Left-hand-side of %s must be an lvalue of type \"int\"",
                  s->kind == is_Incr ? "incrementation" : "decrementation");
        }

        retval.all_branches_return = 0;
        return retval;
    }

    case is_While:
    {
        Expr condexpr = s->u.while_.expr_;
        processed_expr condexpr_e = process_expr(condexpr);

        i32 block_id = push_block();
        Stmt lbody = s->u.while_.stmt_;
        processed_stmt lbody_e = process_stmt(lbody, return_type, block_id);
        (void)lbody_e; // TODO(ir)

        pop_block();

        if (condexpr_e.type_id != TYPEID_BOOL)
        {
            d_type t_given = g_types[TYPEID_UNMASK(condexpr_e.type_id)];
            error(get_lnum(condexpr),
                  "Loop condition has a type %s, when boolean was expected",
                  t_given.name);
        }

        // Don't report missing reutrn if loop condition is always true -
        // function will never reach its end. We don't really care what returns
        // in the body, because we may still never enter the loop itself.
        retval.all_branches_return = (condexpr_e.kind == EET_CONSTANT
                                      && condexpr_e.type_id == TYPEID_BOOL
                                      && condexpr_e.u.cnst.numeric_val == 1);

        return retval;
    }

    case is_For:
    {
        char* type_name = s->u.for_.ident_1;
        char* vname = s->u.for_.ident_2;
        Expr e = s->u.for_.expr_;
        processed_expr e_expr = process_expr(e);

        if (!(e_expr.type_id & TYPEID_FLAG_ARRAY))
        {
            d_type t_given = g_types[TYPEID_UNMASK(e_expr.type_id)];

            error(get_lnum(e), "Type in the \"for\" expresion must be a an array");
            note(get_lnum(e), "Given expression resolved to a type \"%s\"", t_given.name);

            // To avoid error cascade. Since we've erorred anyway, it should work fine
            // Make sure we don't create a void[] type by accident!
            if (e_expr.type_id == TYPEID_VOID)
                e_expr.type_id = TYPEID_INT;

            note(get_lnum(e), "Assuming type \"%s[]\"", g_types[e_expr.type_id].name);
            e_expr.type_id |= TYPEID_FLAG_ARRAY;

        }
        else if ((TYPEID_UNMASK(e_expr.type_id)) == TYPEID_VOID)
        {
            error(get_lnum(e), "Type in the \"for\" expresion must not be a void[]");
            note(get_lnum(e), "Assuming type \"int[]\"");

            // To avoid error cascade. Since we've erorred anyway, it should work fine
            e_expr.type_id = TYPEID_INT | TYPEID_FLAG_ARRAY;
        }

        u32 iter_type_id = symbol_resolve_type(type_name, 0, e);
        assert(iter_type_id == TYPEID_NOTFOUND || !(iter_type_id & TYPEID_FLAG_ARRAY));
        switch (iter_type_id) {
        case TYPEID_NOTFOUND:
        {
            // Default to good iterator type anyway
            iter_type_id = e_expr.type_id & ~TYPEID_FLAG_ARRAY;
            note(get_lnum(e), "Assuming type \"%s\"", g_types[iter_type_id].name);
        } break;
        case TYPEID_VOID:
        {
            error(get_lnum(e), "Cannot declare a variable of type \"void\" in the range loop");

            // Default to good iterator type anyway
            iter_type_id = e_expr.type_id & ~TYPEID_FLAG_ARRAY;
            note(get_lnum(e), "Assuming type \"%s\"", g_types[iter_type_id].name);
        } break;
        default:
        {
        } break;
        }

        if (iter_type_id != (TYPEID_UNMASK(e_expr.type_id)))
        {
            // TODO(ex): Handle inheritance, eg: "for (base b : new dervied[10])"

            d_type t_arr = g_types[TYPEID_UNMASK(e_expr.type_id)];
            d_type t_iter = g_types[TYPEID_UNMASK(iter_type_id)];
            error(get_lnum(e), "Given array has a type, which is non assignable to the iterator type");
            note(get_lnum(e), "Iterator has type \"%s\", when \"%s\" was expected",
                 t_iter.name, t_arr.name);
        }

        i32 iter_block_id = push_block(); // For iterator variable
        d_var var;
        var.lnum = get_lnum(e);
        var.type_id = iter_type_id;
        var.block_id = iter_block_id;
        push_var(var, vname, e);
        i32 block_id = push_block(); // For body

        Stmt lbody = s->u.for_.stmt_;
        processed_stmt lbody_e = process_stmt(lbody, return_type, block_id);
        (void)lbody_e; // TODO(ir)

        pop_block();
        pop_block();

        retval.all_branches_return = 0;
        return retval;
    }
    }

    NOTREACHED; // Should not reach
}

static void
process_func_body(char* fnname, Block b, void* node)
{
    u32 f_id = symbol_resolve_func(fnname, node);
    assert(f_id != FUNCID_NOTFOUND); // Already added

    i32 param_block_id = push_block(); // Block in which params get defined
    d_func_arg* args = g_funcs[f_id].args;
    mm n_args = array_size(args);
    for (mm i = 0; i < n_args; ++i)
    {
        d_var var;
        var.lnum = args[i].lnum;
        var.type_id = args[i].type_id;
        var.block_id = param_block_id;

        push_var(var, args[i].name, node);
    }

    u32 f_rettype_id = g_funcs[f_id].ret_type_id;
    b32 all_branches_return = 0;
    i32 body_block_id = push_block(); // Block where body is parsed (so args can be shadowed)
    assert(b->kind == is_Blk);
    LIST_FOREACH(it, b->u.blk_, liststmt_)
    {
        Stmt st = it->stmt_;
        processed_stmt ev_s = process_stmt(st, f_rettype_id, body_block_id);
        all_branches_return |= ev_s.all_branches_return;
    }

    // One for param_block one for body_block
    pop_block();
    pop_block();

    if (!all_branches_return && f_rettype_id != TYPEID_VOID)
        error(g_funcs[f_id].lnum, "Not all paths return a value in a non-void function");
}

// Get arguments for function. TopDef is assumed to be of the type is_FnDef. If
// this_param is != -1, the first param is called "self" and is of given type
static d_func_arg*
get_args_for_function(ListArg args, i32 this_param)
{
    d_func_arg* fun_args = 0;
    if (this_param != -1)
    {
        d_func_arg arg = { .name = "self", .type_id = (u32)this_param, .lnum = 0 };
        array_push(fun_args, arg);
    }

    struct { ListArg listarg_; } arglist;
    arglist.listarg_ = args;
    LIST_FOREACH(arg_it, arglist, listarg_)
    {
        Arg a = arg_it->arg_;
        assert(a->kind == is_Ar); // Only possible type

        Ident aname = a->u.ar_.ident_;
        parsed_type atype = parse_type(a->u.ar_.type_);
        u32 arg_type_id = symbol_resolve_type(atype.name, atype.is_array, a->u.ar_.type_);

        if (arg_type_id == TYPEID_NOTFOUND)
            arg_type_id = TYPEID_INT;

        if (arg_type_id == TYPEID_VOID || arg_type_id == (TYPEID_VOID | TYPEID_FLAG_ARRAY))
        {
            error(get_lnum(a), "Parameter type void%s is not allowed",
                  arg_type_id & TYPEID_FLAG_ARRAY ? "[]" : "");
            note(get_lnum(a), "Assuming type \"int\"");

            arg_type_id = TYPEID_INT;
        }

        d_func_arg arg = { .name = aname, .type_id = arg_type_id, .lnum = get_lnum(a->u.ar_.type_) };
        array_push(fun_args, arg);
    }

    // Check if all parameter names are unique
    mm n_args = array_size(fun_args);
    if (n_args)
    {
        char** arg_names = 0;
        array_reserve(arg_names, n_args);
        for (mm i = 0; i < n_args; ++i)
            array_push(arg_names, fun_args[i].name);

        qsort(arg_names, (umm)n_args, sizeof(char*), qsort_strcmp);
        for (mm i = 0; i < n_args - 1; ++i)
        {
            if (UNLIKELY(strcmp(arg_names[i], arg_names[i + 1]) == 0))
            {
                error(get_lnum(args), "Redefinition of parameter \"%s\"", arg_names[i]);
                if (this_param != -1 && strcmp(arg_names[i], "self") == 0)
                    note(get_lnum(args), "\"self\" is a implicite param of the member function");

                break;
            }
        }

        array_free(arg_names);
    }

    return fun_args;
}

static void
add_classes(Program p)
{
    LIST_FOREACH(it, p->u.prog_, listtopdef_)
    {
        TopDef t = it->topdef_;
        if (t->kind != is_ClDef)
            continue;

        d_type new_class = {
            .name = t->u.cldef_.ident_,
            .members = 0,
            .member_funcs = 0,
            .lnum = get_lnum(t->u.cldef_.clprops_),
            .is_primitive = 0
        };
        array_push(g_types, new_class);

        symbol s = {
            .type = S_TYPE,
            .id = (i32)(array_size(g_types) - 1)
        };

        b32 shadows = symbol_push(t->u.cldef_.ident_, s);
        if (UNLIKELY(shadows))
        {
            // Since classes are added first, we can be sure, that there only
            // symbol that can collide here is other class' name

            symbol prev_sym = symbol_get_shadowed(t->u.cldef_.ident_);
            assert(prev_sym.type == S_TYPE);

            d_type type = g_types[prev_sym.id];
            if (!type.is_primitive)
            {
                error(new_class.lnum, "Redefinition of class \"%s\"", t->u.cldef_.ident_);
                note(type.lnum, "Previously defined here");
            }
            else
            {
                error(new_class.lnum, "Cannot name a class after a builtin type \"%s\"",
                      t->u.cldef_.ident_);

                symbol_pop(t->u.cldef_.ident_);
            }
        }
    }
}

static void
add_class_members_and_local_funcs(Program p)
{
    LIST_FOREACH(it, p->u.prog_, listtopdef_)
    {
        TopDef t = it->topdef_;
        if (t->kind != is_ClDef)
            continue;

        u32 class_type_id = symbol_resolve_type(t->u.cldef_.ident_, 0, t->u.cldef_.clprops_);
        assert(class_type_id != TYPEID_NOTFOUND); // We've added all classes already

        // Parse member variables
        d_class_mem* members = 0;
        i32 n_members = 0;
        LIST_FOREACH(cl_it, t->u.cldef_, listclbody_)
        {
            ClBody cl = cl_it->clbody_;
            if (cl->kind != is_CBVar)
                continue;

            Type member_type = cl->u.cbvar_.type_;
            char* member_name = cl->u.cbvar_.ident_;

            symbol shad_sym = symbol_check(member_name);
            if (UNLIKELY(shad_sym.type != S_NONE))
            {
                switch (shad_sym.type) {
                case S_TYPE:
                {
                    d_type* type = g_types + shad_sym.id;
                    error(get_lnum(member_type), "Class member \"%s\" cannot be named after a class",
                          member_name);

                    if (shad_sym.id > TYPEID_LAST_BUILTIN_TYPE)
                        note(type->lnum, "Class \"%s\" defined here", member_name);
                    else
                        note(0, "\"%s\" is a builtin type", member_name);
                } break;

                case S_FUN:
                {
                    d_func* func = g_funcs + shad_sym.id;
                    error(get_lnum(member_type), "Class member \"%s\" cannot be named after a function",
                          member_name);

                    if (shad_sym.id > FUNCID_LAST_BUILTIN_FUNC)
                        note(func->lnum, "Function \"%s\" defined here", member_name);
                    else
                        note(0, "\"%s\" is a builtin function", member_name);
                } break;

                case S_VAR:
                case S_NONE:
                    break; // Should not reach
                }
            }

            parsed_type mem_type = parse_type(cl->u.cbvar_.type_);
            u32 type_id = symbol_resolve_type(mem_type.name, mem_type.is_array, cl->u.cbvar_.type_);

            switch (type_id) {
            case TYPEID_NOTFOUND:
            {
                note(get_lnum(cl->u.cbvar_.type_), "Assuming type \"int\"");
                type_id = TYPEID_INT; // Default to int in order to avoid errors
            } break;
            case TYPEID_VOID:
            case TYPEID_VOID | TYPEID_FLAG_ARRAY:
            {
                error(get_lnum(member_type),
                      "Cannot declare member field of type \"void%s\"",
                      type_id & TYPEID_FLAG_ARRAY ? "[]" : "");
                note(get_lnum(member_type), "Assuming type \"int\"");

                type_id = TYPEID_INT;
            } break;
            default:
            {
            } break;
            }

            d_class_mem clmem = {
                .name = member_name,
                .offset = n_members++,
                .type_id = type_id,
                .lnum = get_lnum(member_type),
            };

            array_push(members, clmem);
        }

        if (members)
        {
            // Make it easy to find a member by name
            qsort(members, (umm)n_members, sizeof(d_class_mem), qsort_d_class_mem);
            g_types[class_type_id].members = members;
        }

        // Parse local functions
        d_func* member_funcs = 0;
        LIST_FOREACH(cl_it, t->u.cldef_, listclbody_)
        {
            ClBody cl = cl_it->clbody_;
            if (cl->kind != is_CBFnDef)
                continue;

            void* node = cl->u.cbfndef_.type_;
            char* func_name = cl->u.cbfndef_.ident_;

            symbol shad_sym = symbol_check(func_name);
            if (UNLIKELY(shad_sym.type != S_NONE && shad_sym.type != S_FUN))
            {
                switch (shad_sym.type) {
                case S_TYPE:
                {
                    d_type* type = g_types + shad_sym.id;
                    error(get_lnum(node), "Class method \"%s\" cannot be named after a class",
                          func_name);

                    if (shad_sym.id > TYPEID_LAST_BUILTIN_TYPE)
                        note(type->lnum, "Class \"%s\" defined here", func_name);
                    else
                        note(0, "\"%s\" is a builtin type", func_name);
                } break;

                case S_FUN:
                case S_VAR:
                case S_NONE:
                    break; // Should not reach
                }
            }

            parsed_type retval_type = parse_type(cl->u.cbfndef_.type_);
            u32 type_id = symbol_resolve_type(retval_type.name, retval_type.is_array, cl->u.cbfndef_.type_);
            if (type_id == TYPEID_NOTFOUND)
                type_id = TYPEID_INT;

            if (type_id == (TYPEID_VOID | TYPEID_FLAG_ARRAY))
            {
                error(get_lnum(cl->u.cbfndef_.type_), "Return type void[] is not allowed");
                type_id = TYPEID_INT;
            }

            d_func_arg* fun_args = get_args_for_function(cl->u.cbfndef_.listarg_, (i32)class_type_id);
            mm n_args = array_size(fun_args);

            d_func f = {
                .name = cl->u.cbfndef_.ident_,
                .lnum = get_lnum(cl->u.cbfndef_.type_),
                .ret_type_id = type_id,
                .num_args = (i32)n_args,
                .args = fun_args,
                .is_local = 1,
            };

            array_push(member_funcs, f);
        }

        if (member_funcs)
        {
            // Make it easy to find a member by name
            qsort(member_funcs, (umm)array_size(member_funcs), sizeof(d_func), qsort_d_func);
            g_types[class_type_id].member_funcs = member_funcs;
        }
    }
}

static void
add_global_funcs(Program p)
{
    b32 has_main = 0;
    LIST_FOREACH(it, p->u.prog_, listtopdef_)
    {
        TopDef t = it->topdef_;
        if (t->kind != is_FnDef)
            continue;

        parsed_type retval_type = parse_type(t->u.fndef_.type_);
        u32 type_id = symbol_resolve_type(retval_type.name, retval_type.is_array, t->u.fndef_.type_);
        if (type_id == TYPEID_NOTFOUND)
            type_id = TYPEID_INT;

        if (type_id == (TYPEID_VOID | TYPEID_FLAG_ARRAY))
        {
            error(get_lnum(t->u.fndef_.type_), "Return type void[] is not allowed");
            type_id = TYPEID_INT;
        }

        d_func_arg* fun_args = get_args_for_function(t->u.fndef_.listarg_, -1);
        mm n_args = array_size(fun_args);

        d_func f = {
            .name = t->u.fndef_.ident_,
            .lnum = get_lnum(t->u.fndef_.type_),
            .ret_type_id = type_id,
            .num_args = (i32)n_args,
            .args = fun_args,
            .is_local = 0,
        };

        b32 shadows = create_func(f);
        if (UNLIKELY(shadows))
        {
            symbol prev_sym = symbol_get_shadowed(t->u.fndef_.ident_);
            assert(prev_sym.type == S_FUN || prev_sym.type == S_TYPE);

            if (prev_sym.type == S_FUN)
            {
                d_func prev_f = g_funcs[prev_sym.id];
                error(f.lnum, "Redefinition of global function \"%s\"", t->u.fndef_.ident_);

                if (prev_sym.id > FUNCID_LAST_BUILTIN_FUNC)
                    note(prev_f.lnum, "Previosuly defined here");
                else
                    note(0, "\"%s\" is a builtin function", t->u.fndef_.ident_);
            }
            else
            {
                d_type type = g_types[prev_sym.id];
                if (!type.is_primitive)
                {
                    error(f.lnum, "Function \"%s\" cannot be named same as a class", t->u.fndef_.ident_);
                    note(type.lnum, "Class \"%s\" defined here", t->u.fndef_.ident_);

                    symbol_pop(t->u.fndef_.ident_);
                }
                else
                {
                    error(f.lnum, "Cannot name a function after a builtin type \"%s\"", t->u.fndef_.ident_);
                    symbol_pop(t->u.fndef_.ident_);
                }
            }
        }

        if (!has_main && strcmp("main", t->u.fndef_.ident_) == 0)
        {
            if (f.ret_type_id != TYPEID_INT)
                error(f.lnum, "\"main\" function must return an \"int\"");

            if (n_args != 0)
                error(f.lnum, "\"main\" function must not have any arguments");

            has_main = 1;
        }
    }

    if (!has_main)
    {
        error(0, "No \"main\" function defined");
    }
}

static void
check_global_funcs(Program p)
{
    LIST_FOREACH(it, p->u.prog_, listtopdef_)
    {
        TopDef t = it->topdef_;
        if (t->kind != is_FnDef)
            continue;

        process_func_body(t->u.fndef_.ident_, t->u.fndef_.block_, t->u.fndef_.type_);
    }
}

static void
check_class_funcs(Program p)
{
    LIST_FOREACH(it, p->u.prog_, listtopdef_)
    {
        TopDef t = it->topdef_;
        if (t->kind != is_ClDef)
            continue;

        LIST_FOREACH(cl_it, t->u.cldef_, listclbody_)
        {
            ClBody cl = cl_it->clbody_;
            if (cl->kind != is_CBFnDef)
                continue;

            u32 type_id = symbol_resolve_type(t->u.cldef_.ident_, 0, cl);
            assert(type_id != TYPEID_NOTFOUND);

            d_type type = g_types[type_id];
            mm n_member_funcs = array_size(type.member_funcs);
            for (mm i = 0; i < n_member_funcs; ++i)
                create_func(type.member_funcs[i]);

            push_block(); // Block for class memebers
            mm n_members = array_size(type.members);
            for (mm i = 0; i < n_members; ++i)
            {
                char* vname = type.members[i].name;
                d_var var = {
                    .lnum = type.members[i].lnum,
                    .type_id = type.members[i].type_id,
                    .block_id = -1,
                };

                mm var_id = array_size(g_vars);
                symbol sym;
                sym.type = S_VAR;
                sym.id = (i32)var_id;
                symbol_push(vname, sym);

                array_push(g_vars, var);
                array_push(g_local_symbols, vname);
            }

            process_func_body(cl->u.cbfndef_.ident_, cl->u.cbfndef_.block_, cl->u.cbfndef_.type_);

            pop_block();

            // Pop back the member functions
            for (mm i = 0; i < n_member_funcs; ++i)
                symbol_pop(type.member_funcs[i].name);
            array_popn(g_funcs, n_member_funcs);
        }
    }
}
