#include "absyn.h"
#include "frontend.h"
#include "misc.h"
#include "symtab.h"

static b8* g_inh_matrix;
static mm g_inh_matrix_size;
static i32 g_temp_reg = 1;
static i32 g_label = 1;
static i32 g_local_func_id = 1;

static b32 is_assignable(u32 lhs_type, u32 rhs_type)
{
    if ((lhs_type & TYPEID_FLAG_ARRAY) != (rhs_type & TYPEID_FLAG_ARRAY))
        return 0;

    u32 lhs = TYPEID_UNMASK(lhs_type);
    u32 rhs = TYPEID_UNMASK(rhs_type);
    return g_inh_matrix[rhs * g_inh_matrix_size + lhs];
}

static inline parsed_type
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

static inline processed_stmt
create_empty_statement(void)
{
    processed_stmt retval;
    retval.all_branches_return = 0;

    return retval;
}

static inline processed_expr
compute_binary_boolean_expr(mm v1, mm v2, ir_op op, void* node)
{
    processed_expr retval;
    retval.type_id = TYPEID_BOOL;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;
    retval.is_addr = 0;
    retval.node = node;
    switch (op) {
    case AND: retval.val.u.constant = !!(v1) && !!(v2); break;
    case OR: retval.val.u.constant = !!(v1) || !!(v2); break;
    default: NOTREACHED;
    }

    retval.val.type = IRVT_CONST;
    retval.val.u.constant = retval.val.u.constant;

    return retval;
}

static inline processed_expr
compute_binary_integer_expr(mm v1, mm v2, ir_op op, void* node)
{
    processed_expr retval;
    retval.type_id = TYPEID_INT;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;
    retval.is_addr = 0;
    retval.node = node;

    if (UNLIKELY((op == DIV || op == MOD) && v2 == 0))
    {
        error(get_lnum(node), "Constant division by 0");
        v2 = 1;
    }

    switch (op) {
    case ADD: retval.val.u.constant = v1 + v2; break;
    case SUB: retval.val.u.constant = v1 - v2; break;
    case MUL: retval.val.u.constant = v1 * v2; break;
    case DIV: retval.val.u.constant = v1 / v2; break;
    case MOD: retval.val.u.constant = v1 % v2; break;
    case CMP_LTH: retval.val.u.constant = (v1 < v2); retval.type_id = TYPEID_BOOL; break;
    case CMP_LE: retval.val.u.constant = (v1 <= v2); retval.type_id = TYPEID_BOOL; break;
    case CMP_GTH: retval.val.u.constant = (v1 > v2); retval.type_id = TYPEID_BOOL; break;
    case CMP_GE: retval.val.u.constant = (v1 >= v2); retval.type_id = TYPEID_BOOL; break;
    default: NOTREACHED;
    }

    // Safe for booleans becasue they never overflow. In case of overflow, leave
    // 0 to have something reasonable for computations, we won't compile anyway
    if (UNLIKELY(overflows_32bit(retval.val.u.constant)))
    {
        error(get_lnum(node), "Integer constant overflows");
        retval.val.u.constant = 0;
    }

    retval.val.type = IRVT_CONST;
    retval.val.u.constant = retval.val.u.constant;

    return retval;
}

static inline processed_expr
compute_binary_string_expr(mm str1, mm str2, void* node)
{
    processed_expr retval;
    retval.type_id = TYPEID_STRING;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;
    retval.is_addr = 0;
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

    retval.val.u.constant = strconst_add(s);
    retval.val.type = IRVT_STRCONST;

    return retval;
}

static i32 g_next_block_id = 1;

#define CLMEM_BLOCK_ID (-1)
#define PARAM_BLOCK_ID (1)

static inline i32
push_block(void)
{
    array_push(g_local_symbols, 0); // null is used to makr that the block ends
    return g_next_block_id++;
}

static inline void
pop_block(ir_quadr** ir)
{
    while (1)
    {
        mm size = array_size(g_local_symbols);
        (void)size;
        assert(size > 0);
        char* popped = array_pop(g_local_symbols);
        if (!popped) break;

        symbol s = symbol_get(popped, 0, 0);
        assert(s.type == S_VAR);

        d_var v = g_vars[s.id];
        assert(v.block_id != CLMEM_BLOCK_ID);

        ir_val var_to_dispose = {
            .type = v.argnum >= 0 ?  IRVT_FNPARAM : IRVT_VAR,
            .u = { .reg_id = v.argnum >= 0 ? v.argnum : s.id },
        };

        IR_PUSH(IR_EMPTY(), DISPOSE, var_to_dispose);
        symbol_pop(popped);
    }
}

static inline void
pop_block_nodispose()
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

static inline processed_expr
compute_binary_integer_or_boolean_equality_expr(mm v1, mm v2, ir_op op, void* node)
{
    processed_expr retval;
    retval.type_id = TYPEID_BOOL;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;
    retval.is_addr = 0;
    retval.node = node;
    retval.val.u.constant = !((op == CMP_EQ) ^ (v1 == v2));

    return retval;
}

static inline processed_expr
compute_binary_string_equality_expr(mm str1, mm str2, ir_op op, void* node)
{
    b32 compre = strcmp(g_str_consts[str1].data, g_str_consts[str2].data) == 0;
    processed_expr retval;
    retval.type_id = TYPEID_BOOL;
    retval.kind = EET_CONSTANT;
    retval.is_lvalue = 0;
    retval.is_addr = 0;
    retval.node = node;
    retval.val.u.constant = !((op == CMP_EQ) ^ compre);

    return retval;
}

static inline void
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

static inline b32
expr_requires_jumping_code(Expr e)
{
    if (e->kind == is_EAnd || e->kind == is_EOr) return 1;
    if (e->kind == is_Not) return expr_requires_jumping_code(e->u.not_.expr_);
    return 0;
}

static inline void
typecheck_assigning(u32 type_id, processed_expr* vval, void* node)
{
    if (UNLIKELY(!is_assignable(type_id, vval->type_id)))
    {
        d_type expected_t = g_types[TYPEID_UNMASK(type_id)];
        d_type got_t = g_types[TYPEID_UNMASK(vval->type_id)];
        error(get_lnum(node),
              "Expression of type \"%s%s\" is initialized with incompatible type \"%s%s\"",
              expected_t.name, type_id & TYPEID_FLAG_ARRAY ? "[]" : "",
              got_t.name, vval->type_id & TYPEID_FLAG_ARRAY ? "[]" : "");

        // Clarify why null is not implicitely assignable to a reference type:
        if (vval->type_id == TYPEID_NULL && type_id > TYPEID_LAST_BUILTIN_TYPE)
        {
            note(get_lnum(node),
                 "\"null\" is not implicitly assingable to a reference type. Use: \"(%s)null\"",
                 expected_t.name);
            note(get_lnum(node),
                 "This requirement is forced by the Author of the assignment. Sorry");
        }
    }
}

static void
process_params(ListExpr arg_exprs, d_func* fun, void* node, ir_quadr** ir)
{
    // 1st param "self" for local func. So for local functions we start
    // evaluating params from the second arg. Error messages needs to be
    // adjusted to this hack!
    i32 n_given_args = 0 + !!fun->is_local;
    Expr* arg_rev_exprs = 0;
    struct { ListExpr listexpr_; } expr_list;
    expr_list.listexpr_ = arg_exprs;
    LIST_FOREACH(it, expr_list, listexpr_)
    {
        array_push(arg_rev_exprs, it->expr_);
    }

    // Reverse the arg array
    mm n_given = array_size(arg_rev_exprs);
    mm first = 0;
    mm last = n_given - 1;
    while (first < last)
    {
        Expr temp = arg_rev_exprs[first];
        arg_rev_exprs[first] = arg_rev_exprs[last];
        arg_rev_exprs[last] = temp;
        first++;
        last--;
    }

    for (mm i = 0, size = n_given; i < size; ++i)
    {
        if (LIKELY(n_given_args < fun->num_args)) // if not too many args given
        {
            Expr argexpr = arg_rev_exprs[i];
            u32 expected_type_id = fun->args[
                array_size(fun->args) - n_given_args - 1 + !!fun->is_local].type_id;
            u32 got_type_id;

            if (!expr_requires_jumping_code(argexpr))
            {
                processed_expr argexpr_e = process_expr(argexpr, ir, 0);
                IR_PUSH(IR_EMPTY(), PARAM, argexpr_e.val);

                got_type_id = argexpr_e.type_id;
            }
            else
            {
                preprocessed_jump_expr* pre_buf = 0;
                preprocessed_jump_expr pre = preprocess_jumping_expr(argexpr, &pre_buf, 0);
                got_type_id = TYPEID_BOOL;

                if (pre.kind == PRJE_CONST)
                {
                    ir_val val = IR_CONSTANT(!!(pre.u.constant));
                    IR_PUSH(IR_EMPTY(), PARAM, val);
                }
                else
                {
                    i32 l_true = g_label++;
                    i32 l_false = g_label++;
                    i32 l_end = g_label++;

                    ir_val labl_true = { .type = IRVT_CONST, .u = { .constant = l_true } };
                    ir_val labl_false = { .type = IRVT_CONST, .u = { .constant = l_false } };
                    ir_val labl_end = { .type = IRVT_CONST, .u = { .constant = l_end } };

                    ir_val c0 = IR_CONSTANT(0);
                    ir_val c1 = IR_CONSTANT(1);

                    jump_ctx ctx = { .l_true = l_true, .l_false = l_false };
                    process_jumping_expr(ir, &pre, pre_buf, ctx);

                    IR_PUSH(IR_EMPTY(), LABEL, labl_true);
                    IR_PUSH(IR_EMPTY(), PARAM, c1);
                    IR_PUSH(IR_EMPTY(), JMP, labl_end);
                    IR_PUSH(IR_EMPTY(), LABEL, labl_false);
                    IR_PUSH(IR_EMPTY(), PARAM, c0);
                    IR_PUSH(IR_EMPTY(), LABEL, labl_end);
                }

                array_free(pre_buf);
            }

            if (UNLIKELY(!is_assignable(expected_type_id, got_type_id)))
            {
                d_type t_expected = g_types[TYPEID_UNMASK(expected_type_id)];
                d_type t_provided = g_types[TYPEID_UNMASK(got_type_id)];

                error(get_lnum(argexpr),
                      "Function \"%s\" expects argument %d to have a type \"%s\"",
                      fun->name,
                      n_given_args + 1 - !!(fun->is_local),
                      t_expected.name);

                note(get_lnum(argexpr),
                     "Given expression of type \"%s\", "
                     "which is not assignable to type \"%s\"",
                     t_provided.name,
                     t_expected.name);
            }
        }

        n_given_args++;
    }

    if (n_given_args != fun->num_args)
    {
        error(get_lnum(node),
              "Function \"%s\" takes %d argument%s, but %d %s provided",
              fun->name,
              fun->num_args - !!fun->is_local,
              (fun->num_args - !!fun->is_local) != 1 ? "s" : "",
              n_given_args - !!fun->is_local,
              (fun->num_args - !!fun->is_local) != 1 ? "were" : "was");

        if (fun - g_funcs > FUNCID_LAST_BUILTIN_FUNC)
            note(fun->lnum, "Function \"%s\" defined here", fun->name);
        else
            note(0, "\"%s\" is a builtin function", fun->name);
    }

    array_free(arg_rev_exprs);
}

static processed_expr
process_assignment_expr(Expr e2, ir_val variable_val, ir_quadr** ir, b32 is_addr)
{
    //
    // The assingment of the boolean expression can be expressed as:
    // /* boolean */ b = false;
    // if (compilcated_boolean_expr)
    //     b = true;
    //

    if (!expr_requires_jumping_code(e2))
    {
        processed_expr retval = process_expr(e2, ir, 0);
        IR_ASSIGNMENT_OR_STORE(variable_val, retval.val, is_addr);

        return retval;
    }
    else
    {
        preprocessed_jump_expr* pre_buf = 0;
        preprocessed_jump_expr pre = preprocess_jumping_expr(e2, &pre_buf, 0);

        if (UNLIKELY(pre.kind == PRJE_CONST))
        {
            ir_val val = IR_CONSTANT(!!(pre.u.constant));
            IR_ASSIGNMENT_OR_STORE(variable_val, val, is_addr);

            processed_expr retval;
            retval.node = e2;
            retval.type_id = TYPEID_BOOL;
            retval.kind = EET_CONSTANT;
            retval.is_lvalue = 0;
            retval.is_addr = 0;
            retval.val.u.constant = !!(pre.u.constant);
            retval.val.type = IRVT_CONST;
            retval.val.u.constant = !!(pre.u.constant);

            return retval;
        }

        ir_val c0 = IR_CONSTANT(0);
        IR_ASSIGNMENT_OR_STORE(variable_val, c0, is_addr);

        i32 l_asgn = g_label++;
        i32 l_skip = g_label++;
        ir_val labl_asgn = { .type = IRVT_CONST, .u = { .constant = l_asgn } };
        ir_val labl_skip = { .type = IRVT_CONST, .u = { .constant = l_skip } };

        jump_ctx ctx = { .l_true = l_asgn, .l_false = l_skip };
        process_jumping_expr(ir, &pre, pre_buf, ctx);

        IR_PUSH(IR_EMPTY(), LABEL, labl_asgn);

        ir_val c1 = IR_CONSTANT(1);
        IR_ASSIGNMENT_OR_STORE(variable_val, c1, is_addr);
        IR_PUSH(IR_EMPTY(), LABEL, labl_skip);

        array_free(pre_buf);

        // HACK: This is totally incorrect, but since we only use it with
        //       process_assigning with kind of can live with ot. The only thing
        //       that matters in expr is type
        processed_expr retval;
        retval.node = e2;
        retval.type_id = TYPEID_BOOL;
        retval.kind = EET_COMPUTE;
        retval.is_lvalue = 0;
        retval.is_addr = 0;
        retval.val.u.constant = 0;
        retval.val.type = IRVT_CONST;
        retval.val.u.constant = 0;

        return retval;
    }
}

static processed_expr
process_expr(Expr e, ir_quadr** ir, b32 addr_only)
{
    processed_expr retval;
    Expr binarg1; // To be set for binary expressions
    Expr binarg2;
    ir_op binintop = ADD;
    ir_op binboolop = AND;

    retval.node = e;
    retval.is_lvalue = 0;
    retval.is_addr = 0;
    switch (e->kind) {
    case is_ELitTrue:
    case is_ELitFalse:
    {
        IR_SET_CONSTANT(retval, TYPEID_BOOL, (e->kind == is_ELitTrue));
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

        IR_SET_CONSTANT(retval, TYPEID_INT, e->u.elitint_.integer_);
        return retval;
    }

    case is_ELitStr:
    {
        char* data = e->u.elitstr_.string_;
        string_const c = { .data = data, .len = (mm)strlen(data) };
        mm idx = strconst_add(c);

        IR_SET_STRCONST(retval, TYPEID_STRING, idx);
        return retval;
    }

    case is_Not:
    {
        processed_expr e1 = process_expr(e->u.not_.expr_, ir, 0);
        enforce_type(&e1, TYPEID_BOOL);

        if (UNLIKELY(e1.kind == EET_CONSTANT))
        {
            assert(!e1.is_lvalue);
            e1.val.u.constant = !e1.val.u.constant;
            e1.val.u.constant = e1.val.u.constant;
            return e1;
        }

        IR_SET_EXPR(retval, TYPEID_BOOL, 0, IR_NEXT_TEMP_REGISTER());
        IR_PUSH(retval.val, NOT, e1.val);
        return retval;
    }

    case is_Neg:
    {
        processed_expr e1 = process_expr(e->u.not_.expr_, ir, 0);
        enforce_type(&e1, TYPEID_INT);

        if (UNLIKELY(e1.kind == EET_CONSTANT))
        {
            assert(!e1.is_lvalue);
            e1.val.u.constant = -e1.val.u.constant;
            e1.val.u.constant = e1.val.u.constant;

            // In case when we have -(INT_MIN):
            if (UNLIKELY(overflows_32bit(e1.val.u.constant)))
            {
                error(get_lnum(e1.node), "Integer constant overflows");
                retval.val.u.constant = 0;
            }

            return e1;
        }

        ir_val c0 = IR_CONSTANT(0);
        IR_SET_EXPR(retval, TYPEID_INT, 0, IR_NEXT_TEMP_REGISTER());
        IR_PUSH(retval.val, SUB, c0, e1.val);
        return retval;
    }

    case is_EMul:
    {
        binarg1 = e->u.emul_.expr_1;
        binarg2 = e->u.emul_.expr_2;
        switch (e->u.emul_.mulop_->kind) {
        case is_Times: binintop = MUL; break;
        case is_Div: binintop = DIV; break;
        case is_Mod: binintop = MOD; break;
        }

        goto binary_integer_expr;
    }

    case is_EAdd:
    {
        binarg1 = e->u.eadd_.expr_1;
        binarg2 = e->u.eadd_.expr_2;
        switch (e->u.eadd_.addop_->kind) {
        case is_Plus: binintop = ADD; break;
        case is_Minus: binintop = SUB; break;
        }

        goto binary_integer_expr;
    }

    case is_ERel: // "==" is different expr (EEq), this one is for ints only
    {
        binarg1 = e->u.erel_.expr_1;
        binarg2 = e->u.erel_.expr_2;
        switch (e->u.erel_.relop_->kind) {
        case is_LTH: binintop = CMP_LTH; break;
        case is_GTH: binintop = CMP_GTH; break;
        case is_LE: binintop = CMP_LE; break;
        case is_GE: binintop = CMP_GE; break;
        }

        goto binary_integer_expr;
    }

    case is_EAnd:
    {
        binarg1 = e->u.eand_.expr_1;
        binarg2 = e->u.eand_.expr_2;
        binboolop = AND;

        goto binary_boolean_expr;
    }

    case is_EOr:
    {
        binarg1 = e->u.eor_.expr_1;
        binarg2 = e->u.eor_.expr_2;
        binboolop = OR;

        goto binary_boolean_expr;
    }

    binary_boolean_expr:
    {
        processed_expr e1 = process_expr(binarg1, ir, 0);
        processed_expr e2 = process_expr(binarg2, ir, 0);
        enforce_type(&e1, TYPEID_BOOL);
        enforce_type(&e2, TYPEID_BOOL);

        if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
        {
            assert(!e1.is_lvalue);
            assert(!e2.is_lvalue);

            mm v1 = e1.val.u.constant;
            mm v2 = e2.val.u.constant;
            return compute_binary_boolean_expr(v1, v2, binboolop, e1.node);
        }

        IR_SET_EXPR(retval, TYPEID_BOOL, 0, IR_NEXT_TEMP_REGISTER());
        IR_PUSH(retval.val, binboolop, e1.val, e2.val);
        return retval;
    }

    binary_integer_expr:
    {
        processed_expr e1 = process_expr(binarg1, ir, 0);
        processed_expr e2 = process_expr(binarg2, ir, 0);

        // This is the only case when an operator other that == and != is
        // overloaded. IMO string concatenation should have separate operator
        // (like '@'). Since this is a unique exception, we hack around it:
        if (e1.type_id == TYPEID_STRING && binintop == ADD)
        {
            enforce_type(&e2, TYPEID_STRING);
            if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
            {
                assert(!e1.is_lvalue);
                assert(!e2.is_lvalue);

                mm str1 = e1.val.u.constant;
                mm str2 = e2.val.u.constant;
                return compute_binary_string_expr(str1, str2, e1.node);
            }

            IR_SET_EXPR(retval, TYPEID_STRING, 0, IR_NEXT_TEMP_REGISTER());
            IR_PUSH(retval.val, STR_ADD, e1.val, e2.val);
            return retval;
        }

        enforce_type(&e1, TYPEID_INT);
        enforce_type(&e2, TYPEID_INT);
        if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
        {
            assert(!e1.is_lvalue);
            assert(!e2.is_lvalue);

            mm v1 = e1.val.u.constant;
            mm v2 = e2.val.u.constant;
            return compute_binary_integer_expr(v1, v2, binintop, e1.node);
        }

        u32 typeid = (
            (binintop == CMP_LTH || binintop == CMP_LE
             || binintop == CMP_GTH || binintop == CMP_GE)
            ? TYPEID_BOOL
            : TYPEID_INT);

        IR_SET_EXPR(retval, typeid, 0, IR_NEXT_TEMP_REGISTER());
        IR_PUSH(retval.val, binintop, e1.val, e2.val);
        return retval;
    }

    case is_EEq:
    {
        processed_expr e1 = process_expr(e->u.eeq_.expr_1, ir, 0);
        processed_expr e2 = process_expr(e->u.eeq_.expr_2, ir, 0);
        ir_op bineqop = CMP_EQ;
        switch (e->u.eeq_.eqop_->kind) {
        case is_NE: bineqop = CMP_NEQ; break;
        case is_EQU: bineqop = CMP_EQ; break;
        }

        switch (e1.type_id) {
        case TYPEID_INT:
        {
            if (e2.type_id != TYPEID_INT)
                goto type_missmatch;

            if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
                goto compute_contant_equality_int_or_bool;

            IR_SET_EXPR(retval, TYPEID_BOOL, 0, IR_NEXT_TEMP_REGISTER());
            IR_PUSH(retval.val, bineqop, e1.val, e2.val);
            return retval;
        }

        case TYPEID_BOOL:
        {
            if (e2.type_id != TYPEID_BOOL)
                goto type_missmatch;

            if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
                goto compute_contant_equality_int_or_bool;

            IR_SET_EXPR(retval, TYPEID_BOOL, 0, IR_NEXT_TEMP_REGISTER());
            IR_PUSH(retval.val, bineqop, e1.val, e2.val);
            return retval;
        }

        case TYPEID_STRING:
        {
            if (e2.type_id != TYPEID_STRING)
                goto type_missmatch;

            if (UNLIKELY(e1.kind == EET_CONSTANT && e2.kind == EET_CONSTANT))
                goto compute_contant_equality_string;

            IR_SET_EXPR(retval, TYPEID_BOOL, 0, IR_NEXT_TEMP_REGISTER());
            IR_PUSH(retval.val, bineqop == CMP_EQ ? STR_EQ : STR_NEQ, e1.val, e2.val);
            return retval;
        }

        compute_contant_equality_int_or_bool:
        {
            assert(!e1.is_lvalue);
            assert(!e2.is_lvalue);

            mm v1 = e1.val.u.constant;
            mm v2 = e2.val.u.constant;
            return compute_binary_integer_or_boolean_equality_expr(v1, v2, bineqop, e1.node);
        }

        compute_contant_equality_string:
        {
            assert(!e1.is_lvalue);
            assert(!e2.is_lvalue);

            mm str1 = e1.val.u.constant;
            mm str2 = e2.val.u.constant;
            return compute_binary_string_equality_expr(str1, str2, bineqop, e1.node);
        }

        type_missmatch:
        {
            d_type lhs_t = g_types[TYPEID_UNMASK(e1.type_id)];
            d_type rhs_t = g_types[TYPEID_UNMASK(e2.type_id)];

            error(get_lnum(e1.node), "Comparing expressions with incompatible types");
            note(get_lnum(e1.node), "Left hand side evalutes to \"%s\"", lhs_t.name);
            note(get_lnum(e2.node), "Right hand side evalutes to \"%s\"", rhs_t.name);

            IR_UNDEFINED_EXPR(retval, TYPEID_BOOL, 0);
            return retval;
        }

        compare_voids:
        {
            error(get_lnum(e1.node), "Comparing expressions of type \"void\" is not allowed");

            IR_UNDEFINED_EXPR(retval, TYPEID_BOOL, 0);
            return retval;
        }

        //
        // Latte is unspecified when it comes to comparing the reference type
        // other than strings, so we asssume that this is basically the equality
        // of references (so that equality against null is meaningful).
        //
        default:
        {
            if (UNLIKELY(!(is_assignable(e2.type_id, e1.type_id)
                           || is_assignable(e1.type_id, e2.type_id))))
                goto type_missmatch;

            if (e1.type_id == TYPEID_VOID)
                goto compare_voids;

            IR_SET_EXPR(retval, TYPEID_BOOL, 0, IR_NEXT_TEMP_REGISTER());
            IR_PUSH(retval.val, bineqop, e1.val, e2.val);
            return retval;
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

            IR_UNDEFINED_EXPR(retval, TYPEID_INT, 1);
            return retval;
        }

        d_var var = g_vars[var_id];
        if (var.is_iterator)
        {
            printf("The variable is an iterator, and should be dereferenced first!\n");

            // Must be a variable, b/c function params are not iteratros
            assert(var.block_id != PARAM_BLOCK_ID);

            // TODO: Use addr_only and get only ptr, so that assignment is possible?
            ir_val v = IR_LOCAL_VARIABLE(var_id);
            ir_val zero = { .type = IRVT_CONST, .u = { .reg_id = 0 } };
            IR_SET_EXPR(retval, var.type_id, 0, IR_NEXT_TEMP_REGISTER());
            IR_PUSH(retval.val, SUBSCR, v, zero);

            return retval;
        }

        retval.type_id = var.type_id;
        retval.kind = EET_COMPUTE;
        retval.is_lvalue = 1;
        retval.is_addr = 0;

        if (var.block_id == CLMEM_BLOCK_ID) // Class member
        {
            // First param is "this" pointer
            ir_val self_param_val = { .type = IRVT_FNPARAM, .u = { .reg_id = 0 }, };
            ir_val offset = { .type = IRVT_CONST, .u = { .constant = var.memnum }, };

            IR_SET_EXPR(retval, var.type_id, 1, IR_NEXT_TEMP_REGISTER());
            IR_PUSH(retval.val, addr_only ? ADDROF : SUBSCR, self_param_val, offset);
        }
        else if (var.argnum >= 0) // Func param
        {
            assert(var.block_id == PARAM_BLOCK_ID);
            ir_val val = IR_FUNC_PARAM(var.argnum);
            retval.val = val;
        }
        else // Regular, local variable
        {
            ir_val val = IR_LOCAL_VARIABLE(var_id);
            retval.val = val;
        }

        return retval;
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
        process_params(e->u.eapp_.listexpr_, f, e, ir);

        ir_val fn_to_call = {0};
        if (f->is_local)
        {
            ir_val this_param = { .type = IRVT_FNPARAM, .u = { .constant = 0 }, };
            IR_PUSH(IR_EMPTY(), PARAM, this_param);

            fn_to_call.type = IRVT_LOCFN;
            fn_to_call.u.constant = f->local_id;
        }
        else
        {
            fn_to_call.type = IRVT_FN;
            fn_to_call.u.constant = func_id;
        }

        // Like in Java, functions always return rvalues
        IR_SET_EXPR(retval, f->ret_type_id, 0, IR_NEXT_TEMP_REGISTER());
        IR_PUSH(retval.val, CALL, fn_to_call);
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

        mm n_fields = array_size(g_types[type_id].members);
        ir_val alloc_size = { .type = IRVT_CONST, .u = { .reg_id = n_fields } };
        ir_val constant = { .type = IRVT_CONST, .u = { .constant = type_id } };
        IR_SET_EXPR(retval, type_id, 0, IR_NEXT_TEMP_REGISTER());
        IR_PUSH(retval.val, ALLOC, alloc_size);
        IR_PUSH(IR_EMPTY(), SET_VTAB, retval.val, constant);

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

        processed_expr e_esize = process_expr(esize, ir, 0);
        enforce_type(&e_esize, TYPEID_INT);

        IR_SET_EXPR(retval, type_id, 0, IR_NEXT_TEMP_REGISTER());
        IR_PUSH(retval.val, ALLOC, e_esize.val);
        return retval;
    }

    case is_EArrApp:
    {
        processed_expr e_array = process_expr(e->u.earrapp_.expr_1, ir, 0);
        if (!(e_array.type_id & TYPEID_FLAG_ARRAY))
        {
            error(get_lnum(e->u.earrapp_.expr_1),
                  "Non-array cannot be a left-hand-side of the array subscript");
        }

        processed_expr e_idx = process_expr(e->u.earrapp_.expr_2, ir, 0);
        enforce_type(&e_idx, TYPEID_INT);

        IR_SET_EXPR(retval, TYPEID_UNMASK(e_array.type_id), e_array.is_lvalue, IR_NEXT_TEMP_REGISTER());
        IR_PUSH(retval.val, addr_only ? ADDROF : SUBSCR, e_array.val, e_idx.val);
        return retval;
    }

    case is_EClMem:
    {
        processed_expr e_struct = process_expr(e->u.eclmem_.expr_, ir, 0);
        if (e_struct.type_id & TYPEID_FLAG_ARRAY)
        {
            // In case of array, only .length is possible
            if (UNLIKELY(strcmp(e->u.eclmem_.ident_, "length") != 0))
            {
                error(get_lnum(e->u.eclmem_.expr_),
                      "Member field \"%s\" does not exists on the \"array\" type",
                      e->u.eclmem_.ident_);
            }

            ir_val offset = { .type = IRVT_CONST, .u = { .reg_id = -1 } };
            IR_SET_EXPR(retval, TYPEID_INT, 0, IR_NEXT_TEMP_REGISTER());
            IR_PUSH(retval.val, addr_only ? ADDROF : SUBSCR, e_struct.val, offset);
            return retval;
        }
        else if (e_struct.type_id <= TYPEID_LAST_BUILTIN_TYPE)
        {
            d_type t_provided = g_types[TYPEID_UNMASK(e_struct.type_id)];
            error(get_lnum(e->u.eclmem_.expr_),
                  "Left-hand-side of . expression is not a struct nor array");
            note(get_lnum(e->u.eclmem_.expr_),
                 "Expression type evaluates to \"%s\"", t_provided.name);

            IR_UNDEFINED_EXPR(retval, TYPEID_INT, e_struct.is_lvalue);
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
                retval.is_lvalue = e_struct.is_lvalue;
                retval.kind = EET_COMPUTE;
                return retval;
            }

            ir_val v_idx = { .type = IRVT_CONST, .u = { .constant = idx }, };
            IR_SET_EXPR(retval, retval.type_id, e_struct.is_lvalue, IR_NEXT_TEMP_REGISTER());
            IR_PUSH(retval.val, addr_only ? ADDROF : SUBSCR, e_struct.val, v_idx);

            return retval;
        }

        NOTREACHED;
    }

    case is_EClApp:
    {
        processed_expr e_struct = process_expr(e->u.eclapp_.expr_, ir, 0);
        d_type cltype = g_types[e_struct.type_id];
        char* fn_name = e->u.eclapp_.ident_;

        if (cltype.is_primitive)
        {
            error(get_lnum(e->u.eclapp_.expr_), "Builtin types do not have a member functions");
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
                error(get_lnum(e->u.eclapp_.expr_),
                      "Function \"%s\" is not a member of class \"%s\"",
                      fn_name, cltype.name);
                note(cltype.lnum, "Class \"%s\" defined here", cltype.name);
                note(get_lnum(e->u.eclapp_.expr_), "Assuming type \"int\"");

                retval.type_id = TYPEID_INT;
            }
            else
            {
                d_func* f = cltype.member_funcs + idx;
                process_params(e->u.eclapp_.listexpr_, f, e, ir);

                // TODO(ex): replace with virtual function call

                // Set "this" param:
                ir_val mem_func_id = { .type = IRVT_LOCFN, .u = { .constant = f->local_id }, };
                IR_PUSH(IR_EMPTY(), PARAM, e_struct.val);
                IR_SET_EXPR(retval, retval.type_id, 0, IR_NEXT_TEMP_REGISTER());
                IR_PUSH(retval.val, CALL, mem_func_id);

                return retval;
            }
        }

        retval.is_lvalue = 0; // Like in Java, functions always return rvalues
        retval.kind = EET_COMPUTE;
        return retval;
    }

    case is_ENull:
    {
        IR_SET_CONSTANT(retval, TYPEID_NULL, 0);
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

        processed_expr expr_to_cast = process_expr(e->u.ecast_.expr_, ir, 0);
        switch (expr_to_cast.type_id) {
        case TYPEID_NULL: // Null is always castable to desired type
        {
            IR_SET_CONSTANT(retval, type_id, 0);
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

        retval = expr_to_cast;
        retval.type_id = type_id;
        retval.kind = EET_COMPUTE;
        return retval;
    }
    }

    NOTREACHED;
}

static preprocessed_jump_expr
preprocess_jumping_expr(Expr e, preprocessed_jump_expr** buf, b32 reverse)
{
    switch (e->kind) {
    case is_EAnd:
    case is_EOr:
    {
        preprocessed_jump_expr ee1 = preprocess_jumping_expr(e->u.eand_.expr_1, buf, reverse);
        preprocessed_jump_expr ee2 = preprocess_jumping_expr(e->u.eand_.expr_2, buf, reverse);
        u32 op = (e->kind == is_EAnd) ^ reverse ? BIN_OP_AND : BIN_OP_OR;

        int lhs_const = reverse ? !ee1.u.constant : !!ee1.u.constant;
        int rhs_const = reverse ? !ee2.u.constant : !!ee2.u.constant;

        if (UNLIKELY(ee1.kind == PRJE_CONST && ee2.kind == PRJE_CONST))
        {
            ee1.u.constant =
                ((e->kind == is_EAnd) ^ reverse
                 ? (!!ee1.u.constant) && (!!ee2.u.constant)
                 : (!!ee1.u.constant) || (!!ee2.u.constant));

            return ee1;
        }
        else if (ee1.kind == PRJE_CONST && lhs_const && op == BIN_OP_OR)
        {
            // true || EXPR -> true
            ee1.kind = PRJE_CONST;
            ee1.u.constant = 1;
            ee1.reversed = 0;
            return ee1;
        }
        else if (ee1.kind == PRJE_CONST && !lhs_const && op == BIN_OP_AND)
        {
            // false && EXPR -> false
            ee1.kind = PRJE_CONST;
            ee1.u.constant = 0;
            ee1.reversed = 0;
            return ee1;
        }
        else if (ee2.kind == PRJE_CONST && rhs_const && op == BIN_OP_AND)
        {
            // EXPR && true -> EXPR
            // NOTE: Keep the reversal
            return ee1;
        }
        else if (ee1.kind == PRJE_CONST && lhs_const && op == BIN_OP_AND)
        {
            // true && EXPR -> EXPR
            // NOTE: Keep the reversal
            return ee2;
        }
        else if (ee1.kind == PRJE_CONST && !lhs_const && op == BIN_OP_OR)
        {
            // false || EXPR -> EXPR
            // NOTE: Keep the reversal
            return ee2;
        }
        else if (ee2.kind == PRJE_CONST && !rhs_const && op == BIN_OP_OR)
        {
            // EXPR || false -> EXPR
            // NOTE: Keep the reversal
            return ee1;
        }

        mm nees = array_size(*buf);
        array_push(*buf, ee1);
        array_push(*buf, ee2);

        preprocessed_jump_expr retval = {
            .kind = PRJE_BIN,
            .u = { .bin = { .b1 = nees, .b2 = nees + 1, .op = op } },
            .l_id = g_label++,
            .reversed = reverse,
        };

        return retval;
    }

    case is_Not:
    {
        return preprocess_jumping_expr(e->u.not_.expr_, buf, !reverse);
    }

    case is_ECast:
    case is_EVar:
    case is_ERel:
    case is_EEq:
    case is_ELitTrue:
    case is_ELitFalse:
    case is_EClMem:
    case is_EArrApp:
    case is_EApp:
    case is_EClApp:
    case is_ENew:
    case is_ENewArr:
    case is_ELitInt:
    case is_ELitStr:
    case is_ENull:
    case is_EAdd:
    case is_EMul:
    case is_Neg:
    {
        ir_quadr* dummy_ir = 0;
        processed_expr ee = process_expr(e, &dummy_ir, 0);
        if (UNLIKELY(ee.type_id != TYPEID_BOOL))
        {
            d_type t_got = g_types[ee.type_id];
            error(get_lnum(ee.node),
                  "Expected type \"boolean\" but got incompatible type \"%s\"",
                  t_got.name);
        }

        if (ee.kind == EET_CONSTANT)
        {
            assert(array_size(dummy_ir) == 0);
            array_free(dummy_ir);

            preprocessed_jump_expr retval = {
                .kind = PRJE_CONST,
                .u = { .constant = ee.val.u.constant },
                .l_id = g_label++,
                .reversed = reverse,
            };

            return retval;
        }
        else
        {
            array_free(dummy_ir);

            preprocessed_jump_expr retval = {
                .kind = PRJE_EXPR,
                .u = { .expr = e },
                .l_id = g_label++,
                .reversed = reverse,
            };

            return retval;
        }
    }
    }

    NOTREACHED;
}

static void
process_jumping_expr(ir_quadr** ir,
                     preprocessed_jump_expr* e,
                     preprocessed_jump_expr* const pre_buf,
                     jump_ctx ctx)
{
    switch (e->kind) {
    case PRJE_EXPR:
    {
        ir_val labl_true = { .type = IRVT_CONST, .u = { .constant = e->l_id } };
        IR_PUSH(IR_EMPTY(), LABEL, labl_true);

        processed_expr pe = process_expr(e->u.expr, ir, 0);
        ir_val t_label = { .type = IRVT_CONST, .u = { .constant = ctx.l_true } };
        ir_val f_label = { .type = IRVT_CONST, .u = { .constant = ctx.l_false } };

        IR_PUSH(IR_EMPTY(), e->reversed ? JMP_FALSE : JMP_TRUE, pe.val, t_label);
        IR_PUSH(IR_EMPTY(), JMP, f_label);
    } break;

    case PRJE_BIN:
    {
        preprocessed_jump_expr* lhs_e = pre_buf + e->u.bin.b1;
        preprocessed_jump_expr* rhs_e = pre_buf + e->u.bin.b2;

        ir_val labl_true = { .type = IRVT_CONST, .u = { .constant = e->l_id } };
        IR_PUSH(IR_EMPTY(), LABEL, labl_true);

        jump_ctx ctx1;
        jump_ctx ctx2 = { .l_true = ctx.l_true, .l_false = ctx.l_false };

        if (e->u.bin.op == BIN_OP_OR)
        {
            ctx1.l_true = ctx.l_true;
            ctx1.l_false = rhs_e->l_id;
        }
        else
        {
            ctx1.l_true = rhs_e->l_id;
            ctx1.l_false = ctx.l_false;
        }

        process_jumping_expr(ir, lhs_e, pre_buf, ctx1);
        process_jumping_expr(ir, rhs_e, pre_buf, ctx2);
    } break;

    // In most cases should be optimized-out, but there are corner cases, like
    // if (foo() || true), in which case this constant will be evaludated
    case PRJE_CONST:
    {
        ir_val labl_true = { .type = IRVT_CONST, .u = { .constant = e->l_id } };
        IR_PUSH(IR_EMPTY(), LABEL, labl_true);

        ir_val comparison_result = IR_NEXT_TEMP_REGISTER();
        ir_val to_compare = { .type = IRVT_CONST, .u = { .constant = e->u.constant } };
        ir_val comparison_const = { .type = IRVT_CONST, .u = { .constant = 1 } };
        ir_val t_label = { .type = IRVT_CONST, .u = { .constant = ctx.l_true } };
        ir_val f_label = { .type = IRVT_CONST, .u = { .constant = ctx.l_false } };

        IR_PUSH(comparison_result, CMP_EQ, to_compare, comparison_const);
        IR_PUSH(IR_EMPTY(), e->reversed ? JMP_FALSE : JMP_TRUE, comparison_result, t_label);
        IR_PUSH(IR_EMPTY(), JMP, f_label);
    } break;
    }
}

static processed_stmt
process_stmt(Stmt s, u32 return_type, i32 cur_block_id, ir_quadr** ir)
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
            processed_stmt ev_s = process_stmt(it->stmt_, return_type, new_block_id, ir);
            all_branches_return |= ev_s.all_branches_return;
        }
        pop_block(ir);

        retval.all_branches_return = all_branches_return;
        return retval;
    }

    case is_SExp:
    {
        Expr ex = s->u.sexp_.expr_;
        if (!expr_requires_jumping_code(ex))
        {
            process_expr(ex, ir, 0);
        }
        else
        {
            preprocessed_jump_expr* pre_buf = 0;
            preprocessed_jump_expr pre = preprocess_jumping_expr(ex, &pre_buf, 0);

            // Constant statement-expression can be safetly ignored
            if (pre.kind != PRJE_CONST)
            {
                i32 l_end = g_label++;
                ir_val labl_end = { .type = IRVT_CONST, .u = { .constant = l_end } };

                jump_ctx ctx = { .l_true = l_end, .l_false = l_end };
                process_jumping_expr(ir, &pre, pre_buf, ctx);
                IR_PUSH(IR_EMPTY(), LABEL, labl_end);
            }

            array_free(pre_buf);
        }

        retval.all_branches_return = 0;
        return retval;
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
            char* vname = i->kind == is_NoInit ? i->u.noinit_.ident_ : i->u.init_.ident_;
            processed_expr vval;

            mm var_id = array_size(g_vars);
            ir_val target_variable = IR_LOCAL_VARIABLE(var_id);
            switch (i->kind) {
            case is_NoInit:
            {
                ir_val c0 = IR_CONSTANT(0);
                IR_PUSH(target_variable, MOV, c0);
            } break;
            case is_Init:
            {
                vval = process_assignment_expr(i->u.init_.expr_, target_variable, ir, 0);
                typecheck_assigning(type_id, &vval, i);
            } break;
            }

            // HACK: This works, becasue evaluation the expression cannot create a new var
            assert(var_id == array_size(g_vars));

            d_var var = {
                .lnum = get_lnum(i),
                .type_id = type_id,
                .block_id = cur_block_id,
                .is_iterator = 0,
                .argnum = -1,
                .memnum = -1,
            };

            push_var(var, vname, i);
        }

        retval.all_branches_return = 0;
        return retval;
    }

    case is_Ass:
    {
        // We only need address, not the value of the e1
        processed_expr e1 = process_expr(s->u.ass_.expr_1, ir, 1);
        processed_expr e2 = process_assignment_expr(s->u.ass_.expr_2, e1.val, ir, e1.is_addr);

        if (UNLIKELY(!e1.is_lvalue))
        {
            error(get_lnum(s->u.ass_.expr_1), "Left side of assingment must be an lvalue");
        }

        typecheck_assigning(e1.type_id, &e2, s->u.ass_.expr_2);

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
            e.is_addr = 0;
            e.is_lvalue = 0;

            ir_val empty_val = IR_EMPTY();
            e.val = empty_val;
        }
        else
        {
            e = process_expr(s->u.ret_.expr_, ir, 0);
        }

        if (UNLIKELY(!is_assignable(return_type, e.type_id)))
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

        IR_PUSH(IR_EMPTY(), RET, e.val);

        retval.all_branches_return = 1;
        return retval;
    }

    case is_Cond:
    case is_CondElse:
    {
        processed_stmt s1;
        processed_stmt s2;

        Expr abs_expr = s->kind == is_CondElse ? s->u.condelse_.expr_ : s->u.cond_.expr_;
        Stmt abs_s1 = s->kind == is_CondElse ? s->u.condelse_.stmt_1 : s->u.cond_.stmt_;
        Stmt abs_s2 = s->kind == is_CondElse ? s->u.condelse_.stmt_2 : 0;

        i32 l_true = g_label++;
        i32 l_false = g_label++;
        i32 l_end = g_label++;

        preprocessed_jump_expr* pre_buf = 0;
        preprocessed_jump_expr pre = preprocess_jumping_expr(abs_expr, &pre_buf, 0);

        if (UNLIKELY(pre.kind == PRJE_CONST))
        {
            i32 blk = push_block();
            if (!!(pre.u.constant))
                s1 = process_stmt(abs_s1, return_type, blk, ir);
            else if (s->kind == is_CondElse)
                s1 = process_stmt(abs_s2, return_type, blk, ir);
            else
                s1 = create_empty_statement();
            pop_block(ir);

            retval.all_branches_return = s1.all_branches_return;
            return retval;
        }

        ir_val labl_true = { .type = IRVT_CONST, .u = { .constant = l_true } };
        ir_val labl_false = { .type = IRVT_CONST, .u = { .constant = l_false } };
        ir_val labl_end = { .type = IRVT_CONST, .u = { .constant = l_end } };

        jump_ctx ctx = { .l_true = l_true, .l_false = l_false };
        process_jumping_expr(ir, &pre, pre_buf, ctx);

        IR_PUSH(IR_EMPTY(), LABEL, labl_true);

        i32 blk1 = push_block();
        s1 = process_stmt(abs_s1, return_type, blk1, ir);
        pop_block(ir);

        if (s->kind == is_CondElse)
            IR_PUSH(IR_EMPTY(), JMP, labl_end);

        IR_PUSH(IR_EMPTY(), LABEL, labl_false);

        if (s->kind == is_CondElse)
        {

            i32 blk2 = push_block();
            s2 = process_stmt(abs_s2, return_type, blk2, ir);
            pop_block(ir);

            IR_PUSH(IR_EMPTY(), LABEL, labl_end);
        }
        else
            s2 = create_empty_statement();

        array_free(pre_buf);
        retval.all_branches_return = s1.all_branches_return && s2.all_branches_return;
        return retval;
    }

    case is_Incr:
    case is_Decr:
    {
        Expr e = s->kind == is_Incr ? s->u.incr_.expr_ : s->u.decr_.expr_;
        processed_expr processed_e = process_expr(e, ir, 1);
        if (processed_e.type_id != TYPEID_INT || !processed_e.is_lvalue)
        {
            error(get_lnum(e), "Left-hand-side of %s must be an lvalue of type \"int\"",
                  s->kind == is_Incr ? "incrementation" : "decrementation");
        }

        if (processed_e.is_addr)
        {
            ir_val toadd = { .type = IRVT_CONST, .u = { .constant = s->kind == is_Incr ? 1 : -1 } };
            IR_PUSH(IR_EMPTY(), ADD_AT_ADDR, processed_e.val, toadd);
        }
        else
        {
            ir_val toadd = { .type = IRVT_CONST, .u = { .constant = 1 } };
            IR_PUSH(processed_e.val, (s->kind == is_Incr ? ADD : SUB), processed_e.val, toadd);
        }

        retval.all_branches_return = 0;
        return retval;
    }

    case is_While:
    {
        Expr condexpr = s->u.while_.expr_;
        Stmt lbody = s->u.while_.stmt_;

        i32 l_loop = g_label++;
        i32 l_cond = g_label++;
        i32 l_end = g_label++;
        ir_val labl_loop = { .type = IRVT_CONST, .u = { .constant = l_loop } };
        ir_val labl_cond = { .type = IRVT_CONST, .u = { .constant = l_cond } };
        ir_val labl_end = { .type = IRVT_CONST, .u = { .constant = l_end } };

        b32 all_branches_return = 0;

        preprocessed_jump_expr* pre_buf = 0;
        preprocessed_jump_expr pre = preprocess_jumping_expr(condexpr, &pre_buf, 0);

        if (UNLIKELY(pre.kind == PRJE_CONST))
        {
            all_branches_return = !!(pre.u.constant);

            if (!(pre.u.constant)) // If loop condition is false, we are done
            {
                retval.all_branches_return = 0;
                return retval;
            }
            else // If oop cond is true we can skip checking for the condition
            {
                IR_PUSH(IR_EMPTY(), LABEL, labl_loop);

                i32 block_id = push_block();
                process_stmt(lbody, return_type, block_id, ir);
                pop_block(ir);

                IR_PUSH(IR_EMPTY(), JMP, labl_loop);

                // Don't report missing reutrn if loop condition is always true
                // function will never reach its end
                retval.all_branches_return = 1;
                return retval;
            }
        }

        IR_PUSH(IR_EMPTY(), JMP, labl_cond);
        IR_PUSH(IR_EMPTY(), LABEL, labl_loop);

        i32 block_id = push_block();
        process_stmt(lbody, return_type, block_id, ir);
        pop_block(ir);

        IR_PUSH(IR_EMPTY(), LABEL, labl_cond);

        jump_ctx ctx = { .l_true = l_loop, .l_false = l_end };
        process_jumping_expr(ir, &pre, pre_buf, ctx);

        IR_PUSH(IR_EMPTY(), LABEL, labl_end);

        // Don't report missing reutrn if loop condition is always true -
        // function will never reach its end. We don't really care what returns
        // in the body, because we may still never enter the loop itself.
        retval.all_branches_return = all_branches_return;

        return retval;
    }

    case is_For:
    {
        char* type_name = s->u.for_.ident_1;
        char* vname = s->u.for_.ident_2;
        Expr e = s->u.for_.expr_;
        processed_expr e_expr = process_expr(e, ir, 0);

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

        if (!is_assignable(iter_type_id, TYPEID_UNMASK(e_expr.type_id)))
        {
            d_type t_arr = g_types[TYPEID_UNMASK(e_expr.type_id)];
            d_type t_iter = g_types[TYPEID_UNMASK(iter_type_id)];
            error(get_lnum(e), "Given array has a type, which is non assignable to the iterator type");
            note(get_lnum(e), "Iterator has type \"%s\", when \"%s\" was expected",
                 t_iter.name, t_arr.name);
        }

        i32 iter_block_id = push_block(); // For iterator variable
        mm iterator_id = array_size(g_vars);
        d_var var = {
            .lnum = get_lnum(e),
            .type_id = iter_type_id,
            .block_id = iter_block_id,
            .is_iterator = 1,
            .argnum = -1,
            .memnum = -1,
        };
        push_var(var, vname, e);

        // Set iterator to the start of the array and temp var for its end
        ir_val loop_end = IR_NEXT_TEMP_REGISTER();
        ir_val iterator = { .type = IRVT_VAR, .u = { .reg_id = iterator_id } };
        IR_PUSH(loop_end, ARR_SET_END, e_expr.val);
        IR_PUSH(iterator, MOV, e_expr.val);

        i32 l_loop = g_label++;
        i32 l_cond = g_label++;
        ir_val labl_loop = { .type = IRVT_CONST, .u = { .constant = l_loop } };
        ir_val labl_cond = { .type = IRVT_CONST, .u = { .constant = l_cond } };
        i32 block_id = push_block(); // For body

        IR_PUSH(IR_EMPTY(), JMP, labl_cond);
        IR_PUSH(IR_EMPTY(), LABEL, labl_loop);

        Stmt lbody = s->u.for_.stmt_;
        process_stmt(lbody, return_type, block_id, ir);

        // Increment the iterator check the condition and continue if needed
        ir_val toadd = { .type = IRVT_CONST, .u = { .constant = 8 } };
        IR_PUSH(iterator, ADD, iterator, toadd);

        ir_val comparison_result = IR_NEXT_TEMP_REGISTER();
        IR_PUSH(IR_EMPTY(), LABEL, labl_cond);
        IR_PUSH(comparison_result, CMP_EQ, iterator, loop_end);
        IR_PUSH(IR_EMPTY(), JMP_FALSE, comparison_result, labl_loop);

        pop_block(ir);
        pop_block(ir);

        retval.all_branches_return = 0;
        return retval;
    }
    }

    NOTREACHED; // Should not reach
}

static void
process_func_body(char* fnname, Block b, void* node)
{
    g_temp_reg = 1;
    g_next_block_id = 1;

    u32 f_id = symbol_resolve_func(fnname, node);
    assert(f_id != FUNCID_NOTFOUND); // Already added

    i32 param_block_id = push_block(); // Block in which params get defined
    assert(param_block_id == PARAM_BLOCK_ID);

    d_func_arg* args = g_funcs[f_id].args;
    mm n_args = array_size(args);
    for (mm i = 0; i < n_args; ++i)
    {
        d_var var = {
            .lnum = args[i].lnum,
            .type_id = args[i].type_id,
            .block_id = param_block_id,
            .is_iterator = 0,
            .argnum = (i32)i,
            .memnum = -1,
        };

        push_var(var, args[i].name, node);
    }

    u32 f_rettype_id = g_funcs[f_id].ret_type_id;
    b32 all_branches_return = 0;
    i32 body_block_id = push_block(); // Block where body is parsed (so args can be shadowed)
    ir_quadr* ircode = 0;
    ir_quadr** ir = &ircode; // To make the hacky macros works
    i32 return_label_id = g_label++; // Label of the return code

    assert(b->kind == is_Blk);
    LIST_FOREACH(it, b->u.blk_, liststmt_)
    {
        Stmt st = it->stmt_;
        processed_stmt ev_s = process_stmt(st, f_rettype_id, body_block_id, &ircode);
        all_branches_return |= ev_s.all_branches_return;
    }

    // One for param_block one for body_block
    pop_block(ir);
    pop_block(ir);

    if (!all_branches_return && f_rettype_id != TYPEID_VOID)
        error(g_funcs[f_id].lnum, "Not all paths return a value in a non-void function");

    // HACK: Move RET _after_ last dispose'es, so that removing unnecesarry
    //       branching will work correctly
    mm n_quards = array_size(ircode);
    mm idx = n_quards - 1;
    for (; idx >= 0; --idx)
    {
        if (ircode[idx].op == RET)
        {
            if (idx != n_quards - 1)
            {
                ir_quadr retq = ircode[idx];
                memmove(ircode + idx, ircode + idx + 1, sizeof(ir_quadr) * (n_quards - 1 - idx));
                ircode[n_quards - 1] = retq;
            }

            break;
        }

        if (ircode[idx].op != DISPOSE) break;
    }

    // Return is emmited after this label
    ir_val empty = IR_EMPTY();
    ir_val return_label = IR_CONSTANT(return_label_id);
    IR_PUSH(empty, LABEL, return_label);

    g_funcs[f_id].return_label_id = return_label_id;
    g_funcs[f_id].code = ircode;
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
            .body = t->u.cldef_.listclbody_,
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

static i32*
build_inhtree(Program p)
{
    i32* retval = malloc(array_size(g_types) * sizeof(i32));
    LIST_FOREACH(it, p->u.prog_, listtopdef_)
    {
        TopDef t = it->topdef_;
        if (t->kind != is_ClDef)
            continue;

        symbol ts = symbol_get(t->u.cldef_.ident_, t->u.cldef_.clprops_, 1);
        assert(ts.type == S_TYPE);

        i32 parent_class_id = -1;
        if (t->u.cldef_.clprops_->kind == is_CExtends)
        {
            symbol is = symbol_get(t->u.cldef_.clprops_->u.cextends_.ident_,
                                  t->u.cldef_.clprops_, 1);
            if (is.type == S_NONE) return 0; // Inherited class not found

            assert(is.type == S_TYPE);
            parent_class_id = is.id;

            if (UNLIKELY(parent_class_id <= TYPEID_LAST_BUILTIN_TYPE))
                error(get_lnum(t->u.cldef_.clprops_), "Cannot inherit from a builtin type");
        }

        retval[ts.id] = parent_class_id;
    }

    for (mm i = 0; i <= TYPEID_LAST_BUILTIN_TYPE; ++i)
        retval[i] = -1;

    return retval;
}

static void
add_class_members_and_local_funcs(i32* types, i32* exts)
{
    for (mm i = 0, n_types = array_size(types); i < n_types; ++i)
    {
        if (i <= TYPEID_LAST_BUILTIN_TYPE)
            continue;

        u32 type_id = types[i];
        d_type* type = g_types + type_id;
        printf("Parsing %s\n", type->name);

        //
        // Parse member variables
        //

        d_class_mem* members = 0;
        i32 n_members = 0;

        if (exts[type_id] != -1)
        {
            d_class_mem* parent_members = g_types[exts[type_id]].members;
            mm n_parent_members = array_size(parent_members);
            if (n_parent_members > 0)
                array_pushn(members, parent_members, n_parent_members);
            n_members = (i32)array_size(members);
        }

        struct { ListClBody listclbody_; } clbody_list;
        clbody_list.listclbody_ = type->body;
        LIST_FOREACH(cl_it, clbody_list, listclbody_)
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
                    d_type* oth_type = g_types + shad_sym.id;
                    error(get_lnum(member_type), "Class member \"%s\" cannot be named after a class",
                          member_name);

                    if (shad_sym.id > TYPEID_LAST_BUILTIN_TYPE)
                        note(oth_type->lnum, "Class \"%s\" defined here", member_name);
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
            u32 mem_type_id = symbol_resolve_type(mem_type.name, mem_type.is_array, cl->u.cbvar_.type_);

            switch (mem_type_id) {
            case TYPEID_NOTFOUND:
            {
                note(get_lnum(cl->u.cbvar_.type_), "Assuming type \"int\"");
                mem_type_id = TYPEID_INT; // Default to int in order to avoid errors
            } break;
            case TYPEID_VOID:
            case TYPEID_VOID | TYPEID_FLAG_ARRAY:
            {
                error(get_lnum(member_type),
                      "Cannot declare member field of type \"void%s\"",
                      mem_type_id & TYPEID_FLAG_ARRAY ? "[]" : "");
                note(get_lnum(member_type), "Assuming type \"int\"");

                mem_type_id = TYPEID_INT;
            } break;
            default:
            {
            } break;
            }

            d_class_mem clmem = {
                .name = member_name,
                .offset = n_members++,
                .type_id = mem_type_id,
                .lnum = get_lnum(member_type),
            };

            array_push(members, clmem);
        }

        // TODO: Get rid of these, but actually CHECK IF MEMBERS ARE UNIQUE,
        //       becasue it is not checked right now
#if 0
        if (members)
        {
            // Make it easy to find a member by name
            qsort(members, (umm)n_members, sizeof(d_class_mem), qsort_d_class_mem);
            g_types[mem_type_id].members = members;
        }
#else
        g_types[type_id].members = members;
#endif

        //
        // Parse local functions
        //

        d_func* member_funcs = 0;
        d_func* inherited_funcs = 0;
        if (exts[type_id] != -1)
        {
            inherited_funcs = g_types[exts[type_id]].member_funcs;

            mm n_inherited_funcs = array_size(inherited_funcs);
            array_pushn(member_funcs, inherited_funcs, n_inherited_funcs);
            for (mm f = 0; f < n_inherited_funcs; ++f)
                member_funcs[f].local_body = 0;
        }

        LIST_FOREACH(cl_it, clbody_list, listclbody_)
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
                    d_type* oth_type = g_types + shad_sym.id;
                    error(get_lnum(node), "Class method \"%s\" cannot be named after a class",
                          func_name);

                    if (shad_sym.id > TYPEID_LAST_BUILTIN_TYPE)
                        note(oth_type->lnum, "Class \"%s\" defined here", func_name);
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
            u32 ret_type_id = symbol_resolve_type(retval_type.name, retval_type.is_array, cl->u.cbfndef_.type_);
            if (ret_type_id == TYPEID_NOTFOUND)
                ret_type_id = TYPEID_INT;

            if (ret_type_id == (TYPEID_VOID | TYPEID_FLAG_ARRAY))
            {
                error(get_lnum(cl->u.cbfndef_.type_), "Return type void[] is not allowed");
                ret_type_id = TYPEID_INT;
            }

            d_func_arg* fun_args = get_args_for_function(cl->u.cbfndef_.listarg_, (i32)type_id);
            mm n_args = array_size(fun_args);

            //
            // TODO: If a function is to acutally hide a parent function, check
            //       the return type and params and insead of adding new one,
            //       replace clbody in the existing one
            //
            mm inhf = 0;
            mm n_inherited_funcs = array_size(inherited_funcs);
            for (; inhf < n_inherited_funcs; ++inhf)
                if (strcmp(inherited_funcs[inhf].name, cl->u.cbfndef_.ident_) == 0)
                    break;

            d_func f = {
                .name = cl->u.cbfndef_.ident_,
                .lnum = get_lnum(cl->u.cbfndef_.type_),
                .ret_type_id = ret_type_id,
                .num_args = (i32)n_args,
                .args = fun_args,
                .local_body = cl,
                .is_local = 1,
                .local_id = g_local_func_id++,
            };

            if (inhf != n_inherited_funcs)
            {
                printf("Shadowing function %s\n", cl->u.cbfndef_.ident_);
                member_funcs[inhf] = f;
            }
            else
            {
                printf("Defining a new function %s\n", cl->u.cbfndef_.ident_);
                array_push(member_funcs, f);
            }
        }

#if 0
        // TODO: Remove that?
        // Make it easy to find a member by name
        if (member_funcs)
            qsort(member_funcs, (umm)array_size(member_funcs), sizeof(d_func), qsort_d_func);
#endif

        g_types[type_id].member_funcs = member_funcs;
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
            .local_id = -1,
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

        array_clear(g_vars);
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

        u32 type_id = symbol_resolve_type(t->u.cldef_.ident_, 0, t);
        assert(type_id != TYPEID_NOTFOUND);

        d_type type = g_types[type_id];
        mm n_member_funcs = array_size(type.member_funcs);

        for (mm mf = 0; mf < n_member_funcs; ++mf)
        {
            ClBody cl = type.member_funcs[mf].local_body;
            if (!cl) continue; // Taking the inherited function

            create_func(type.member_funcs[mf]);
        }

        for (mm mf = 0; mf < n_member_funcs; ++mf)
        {
            ClBody cl = type.member_funcs[mf].local_body;
            if (!cl) continue; // Taking the inherited function

            array_clear(g_vars);

            push_block(); // Block for class memebers
            mm n_members = array_size(type.members);
            for (mm i = 0; i < n_members; ++i)
            {
                char* vname = type.members[i].name;
                d_var var = {
                    .lnum = type.members[i].lnum,
                    .type_id = type.members[i].type_id,
                    .block_id = -1,
                    .is_iterator = 0,
                    .argnum = -1,
                    .memnum = (i32)i,
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
            pop_block_nodispose();
        }

        // Pop back the member functions
        for (mm mf = 0; mf < n_member_funcs; ++mf)
        {
            ClBody cl = type.member_funcs[mf].local_body;
            if (!cl) continue; // Taking the inherited function

            symbol_pop(type.member_funcs[mf].name);
        }
    }
}
