#include "codegen.h"
#include "frontend.h"
#include "optim.h"

#if 0
static void
opt_ctx_add_used_label(i64 label_id, b8** used_labels)
{
    mm used_labels_size = array_size(*used_labels);
    if (label_id >= used_labels_size)
    {
        mm to_insert = label_id - used_labels_size + 1;
        b8 booleans[to_insert];
        memset(booleans, 0, sizeof(b8) * (umm)to_insert);

        array_pushn(*used_labels, booleans, to_insert);
    }

    (*used_labels)[label_id] = 1;
}

enum used_irval_t
{
    UIT_V = 0,
    UIT_P = 1,
    UIT_T = 2,
};
typedef enum used_irval_t used_irval_t;

typedef struct used_irval used_irval;
struct used_irval
{
    u64 regnum;
    used_irval_t type;
};
#endif

#if 0
static void
add_used_var(ir_val v, used_irval** used_vals)
{
    used_irval uv;
    switch (v.type) {
    case IRVT_VAR:
    {
        uv.type = UIT_V;
        uv.regnum = v.u.reg_id;
    } break;
    case IRVT_FNPARAM:
    {
        uv.type = UIT_P;
        uv.regnum = v.u.reg_id;
    } break;
    case IRVT_TEMP:
    {
        uv.type = UIT_T;
        uv.regnum = v.u.reg_id;
    } break;

    case IRVT_NONE:
    case IRVT_FN:
    case IRVT_CONST:
    case IRVT_STRCONST:
    NOTREACHED;
    }

    mm used_vals_size = array_size(*used_vals);
    mm i = 0;
    for (; i < used_vals_size; ++i)
    {
        if (uv.type == (*used_vals)[i].type
            && uv.regnum == (*used_vals)[i].regnum)
        {
            break;
        }
    }

    if (i == used_vals_size) // Not found
        array_push(*used_vals, uv);
}
#endif

static int
qsort_life_interval(void const* lhs_p, void const* rhs_p)
{
    life_interval const* lhs = (life_interval const*)(lhs_p);
    life_interval const* rhs = (life_interval const*)(rhs_p);

    mm d1 = lhs->end - rhs->end;
    d1 = d1 != 0 ? d1 : rhs->start - lhs->start;
    return d1 < 0 ? -1 : (d1 > 0 ? 1 : 0);
}

static void
lifetime_free(lifetime_info* info)
{
    free(info->mem);
}

// Return true, if variable of type and register as given is used at programs
// point ino. Type can be only V, T or P.
static b8
lifetime_check_at(lifetime_info* info, mm reg_id, ir_val_type type, mm ino)
{
    b8* row = info->mem + (info->n_all * ino);
    switch (type) {
    case IRVT_VAR: NOTREACHED; // TODO
    case IRVT_FNPARAM: NOTREACHED; // TODO
    case IRVT_TEMP: return *(row + info->n_vars + info->n_fparams + reg_id);
    case IRVT_NONE:
    case IRVT_FN:
    case IRVT_CONST:
    case IRVT_STRCONST:
        NOTREACHED;
    }
}

static lifetime_info
compute_lifetimes(ir_quadr* ir)
{
    mm n_vars = 0;
    mm n_fparams = 0;
    mm n_temps = 0;
    mm ir_size = array_size(ir);
    for (mm i = 0; i < ir_size; ++i)
    {
        // TODO: Clear copypaste
        {
            ir_val v = ir[i].target;
            switch (v.type) {
            case IRVT_VAR:
            {
                n_vars = MAX(n_vars, v.u.reg_id + 1);
            } break;
            case IRVT_FNPARAM:
            {
                n_fparams = MAX(n_fparams, v.u.reg_id + 1);
            } break;
            case IRVT_TEMP:
            {
                n_temps = MAX(n_temps, v.u.reg_id + 1);
            } break;
            case IRVT_NONE:
            case IRVT_FN:
            case IRVT_CONST:
            case IRVT_STRCONST:
            {
            } break;
            }
        }

        mm n_args = ir_op_n_args[ir[i].op];
        for (mm a = 0; a < n_args; ++a)
        {
            ir_val v = ir[i].u.args[a];
            switch (v.type) {
            case IRVT_VAR:
            {
                n_vars = MAX(n_vars, v.u.reg_id + 1);
            } break;
            case IRVT_FNPARAM:
            {
                n_fparams = MAX(n_fparams, v.u.reg_id + 1);
            } break;
            case IRVT_TEMP:
            {
                n_temps = MAX(n_temps, v.u.reg_id + 1);
            } break;
            case IRVT_NONE:
            case IRVT_FN:
            case IRVT_CONST:
            case IRVT_STRCONST:
            {
            } break;
            }
        }
    }

#define SET_VAR(VAL) *(row + v.u.reg_id) = (VAL)
#define SET_FNPARAM(VAL) *(row + n_vars + v.u.reg_id) = (VAL)
#define SET_TEMP(VAL) *(row + n_vars + n_fparams + v.u.reg_id) = (VAL)

    mm n_all = n_vars + n_fparams + n_temps;
    b8* all = calloc(n_all * ir_size, sizeof(b8));
    for (mm i = ir_size - 1; i >= 0; --i)
    {
        if (i < ir_size - 1)
            memcpy((all + n_all * i), (all + n_all * (i + 1)), sizeof(b8) * n_all);

        b8* row = all + (n_all * i);

        {
            ir_val v = ir[i].target;
            switch (v.type) {
            case IRVT_VAR: SET_VAR(0); break;
            case IRVT_FNPARAM: SET_FNPARAM(0); break;
            case IRVT_TEMP: SET_TEMP(0); break;
            case IRVT_NONE:
            case IRVT_FN:
            case IRVT_CONST:
            case IRVT_STRCONST:
                break;
            }
        }

        for (mm a = 0, n_args = ir_op_n_args[ir[i].op]; a < n_args; ++a)
        {
            ir_val v = ir[i].u.args[a];
            switch (v.type) {
            case IRVT_VAR: SET_VAR(1); break;
            case IRVT_FNPARAM: SET_FNPARAM(1); break;
            case IRVT_TEMP: SET_TEMP(1); break;
            case IRVT_NONE:
            case IRVT_FN:
            case IRVT_CONST:
            case IRVT_STRCONST:
                break;
            }
        }
    }

    lifetime_info retval = {
        .mem = all,
        .n_all = n_all,
        .n_vars = n_vars,
        .n_fparams = n_fparams,
        .n_temps = n_temps,
    };

    return retval;
}

static reg_alloc_info
allocate_registers_for_temps(ir_quadr* ir, lifetime_info* info)
{
    u8* alloc_info = calloc(info->n_all, sizeof(u8)); // 0 means no register
    mm ir_size = array_size(ir);
    life_interval* intervs = 0;
    b8 dead[info->n_temps + 1]; // +1 because zero-length vlas are in fact UB
    memset(dead, 0, sizeof(b8) * (info->n_temps + 1));

    for (mm i = 0; i < info->n_temps; ++i)
    {
        mm life_starts = 0;
        mm life_ends = 0;

        mm ino = 0;
        while (ino < ir_size && !lifetime_check_at(info, i, IRVT_TEMP, ino))
            ++ino;
        life_starts = ino;

        while (ino < ir_size && lifetime_check_at(info, i, IRVT_TEMP, ino))
            ++ino;
        life_ends = ino;

        if (life_starts == life_ends && life_ends == ir_size)
            dead[i] = 1;

        if (!dead[i])
        {
            life_interval in = {
                .id = i,
                .start = life_starts,
                .end = life_ends,
            };

            array_push(intervs, in);
        }
    }

    // Replace dead targets with empty expr
    for (mm i = 0; i < ir_size; ++i)
    {
        if (ir[i].target.type == IRVT_TEMP && dead[ir[i].target.u.reg_id])
        {
            if (ir[i].op == CALL) // Other side-effect operations
            {
                ir_val empty = IR_EMPTY();
                ir[i].target = empty;
            }
            else // Non-effect operations are simply skipped
            {
                ir_val empty = IR_EMPTY();
                ir[i].target = empty;
                ir[i].op = NOP;
            }
        }
    }

    mm intervs_size = array_size(intervs);
    if (intervs_size > 0)
        qsort(intervs, intervs_size, sizeof(life_interval), qsort_life_interval);

    life_interval* regs[X64_NUM_REGS] = {0};
    mm* unallocated = 0;
    mm first_al = RCX;
    mm regs_size = first_al + MAX_ALLOCATED_REGS;
    assert(regs_size <= X64_NUM_REGS);

    for (mm i = 0, size = intervs_size; i < size; ++i)
    {
        life_interval life = intervs[i];
        mm r = first_al;
        for (; r < regs_size; ++r)
        {
            // TODO: MAYBE replace end < life.start with end <= life.start ?
            //       Will it break code?
            mm n_interv_alloced = array_size(regs[r]);
            if (n_interv_alloced == 0 || regs[r][n_interv_alloced - 1].end < life.start) // TODO: Double check
            {
                array_push(regs[r], life);
                break;
            }
        }

        if (r == regs_size)
            array_push(unallocated, life.id);
        printf("%ld: [%ld; %ld]\n", life.id, life.start, life.end);
    }

    printf("\nREGISTER ALLOCATION:\n");
    for (mm r = first_al; r < regs_size; ++r)
    {
        printf("%s:\n", x64_reg_name[r]);
        for (mm j = 0; j < array_size(regs[r]); ++j)
        {
            alloc_info[info->n_vars + info->n_fparams + regs[r][j].id] = (u8)r;
            printf("    t_%ld: [%ld; %ld]\n", regs[r][j].id, regs[r][j].start, regs[r][j].end);
        }

        printf("\n");
    }

    printf("ALLOCATED:\n");
    for (mm i = 0; i < info->n_temps; ++i)
    {
        mm alloced_reg_no = alloc_info[info->n_vars + info->n_fparams + i];
        if (alloced_reg_no)
            printf("    %s <- t_%ld\n", x64_reg_name[alloced_reg_no], i);
    }

    printf("UNALLOCATED:\n");
    for (mm i = 0, unallocated_size = array_size(unallocated); i < unallocated_size; ++i)
    {
        printf("    t_%ld\n", unallocated[i]);
    }

    reg_alloc_info retval = {
        .vars = alloc_info,
        .params = alloc_info + info->n_vars,
        .temps = alloc_info + info->n_vars + info->n_fparams,
    };

    return retval;
}

static opt_ctx
opt_create_ctx(ir_quadr* ir)
{
    mm ir_size = array_size(ir);
    mm used_labels_size = (g_label + 1);
    b8* used_labels = malloc(sizeof(b8) * used_labels_size); // Num of all labels
    memset(used_labels, 0, sizeof(b8) * used_labels_size);

    for (mm i = 0; i < ir_size; ++i)
    {
        if (ir[i].op == JMP)
            used_labels[ir[i].u.args[0].u.constant] = 1;
        else if (ir[i].op == JMP_TRUE || ir[i].op == JMP_FALSE)
            used_labels[ir[i].u.args[1].u.constant] = 1;
    }

    for (mm i = 0; i < used_labels_size; ++i)
    {
        if (used_labels[i])
            printf("Used label: %ld\n", i);
    }

    opt_ctx retval = {
        .used_labels = used_labels,
    };

    return retval;
}

// Remove fallthrough JMP instructions, so that:
// JMP_TRUE (something) L2
// JMP L1
// L2:
//
// is turned to:
// JMP_FALSE (something) L1
// L2:
//
// ... and ...
// JMP_TRUE (something) L1
// JMP L2
// L2:
//
// is turned to:
// JMP_TRUE (something) L1
// L2:
//
// TODO: Actually implement the second one (find the case for it in the tests first)
static void
opt_fallthrough_jumps(ir_quadr** ir)
{
    ir_quadr* new_ir = 0;
    ir_quadr* cur_ir = *ir;
    mm cur_ir_size = array_size(cur_ir);
    array_reserve(new_ir, cur_ir_size);

    for (mm i = 0; i < cur_ir_size; ++i)
    {
        if (LIKELY(i + 2 < cur_ir_size)
            && (cur_ir[i].op == JMP_TRUE || cur_ir[i].op == JMP_FALSE)
            && cur_ir[i + 1].op == JMP
            && cur_ir[i + 2].op == LABEL
            && cur_ir[i].u.args[1].u.constant == cur_ir[i + 2].u.args[0].u.constant)
        {
            printf("Being fancy here!\n");
            ir_quadr cjmp = cur_ir[i];
            cjmp.op = cjmp.op == JMP_TRUE ? JMP_FALSE : JMP_TRUE;
            cjmp.u.args[1].u.constant = cur_ir[i + 1].u.args[0].u.constant;

            array_push(new_ir, cjmp);
            array_push(new_ir, cur_ir[i + 2]); // Push label, because it does not change

            i += 2;
            continue;
        }

        array_push(new_ir, cur_ir[i]);
    }

    array_free(cur_ir);
    *ir = new_ir;
}

// Remove labels that nobody ever jumps to
static void
opt_del_dead_labels(ir_quadr** ir, opt_ctx* ctx)
{
    ir_quadr* new_ir = 0;
    ir_quadr* cur_ir = *ir;
    mm cur_ir_size = array_size(cur_ir);
    array_reserve(new_ir, cur_ir_size);

    for (mm i = 0; i < cur_ir_size; ++i)
    {
        if (cur_ir[i].op == LABEL && !ctx->used_labels[cur_ir[i].u.args[0].u.constant])
            continue;

        array_push(new_ir, cur_ir[i]);
    }

    array_free(cur_ir);
    *ir = new_ir;
}

static ir_op
get_correct_jump_op(ir_op original_op, b32 is_neg)
{
    switch (original_op) {
    case CMP_EQ: return !is_neg ? JMP_FLAGS_E : JMP_FLAGS_NE;
    case CMP_NEQ: return !is_neg ? JMP_FLAGS_NE : JMP_FLAGS_E;
    case CMP_LTH: return !is_neg ? JMP_FLAGS_L : JMP_FLAGS_GE;
    case CMP_LE: return !is_neg ? JMP_FLAGS_LE : JMP_FLAGS_G;
    case CMP_GTH: return !is_neg ? JMP_FLAGS_G : JMP_FLAGS_LE;
    case CMP_GE: return !is_neg ? JMP_FLAGS_GE : JMP_FLAGS_L;
    default: NOTREACHED;
    }
}

//
// This function optimizes jumping code like:
//
//   t_2 = CMP_LTH v_0 v_1
//   JMP_TRUE t_2 LAB ; t_2 is last time alive
//
// Into:
//   CMP v_0 v_1
//   JMP_FLAGS_LTH LAB
//
// Which allows us to take advnantage of x64 conditional jump instructions,
// removing a temp register
//
static void
replace_compare_jumps_with_flag_jumps(ir_quadr** ir_, lifetime_info* info)
{
    ir_quadr* ir = *ir_; // we don't add/remove elems from the array
    for (mm i = 0, ir_size = array_size(ir); i < ir_size - 2; ++i)
    {
        if ((ir[i].op == CMP_LTH || ir[i].op == CMP_GTH
             || ir[i].op == CMP_LE || ir[i].op == CMP_GE
             || ir[i].op == CMP_EQ || ir[i].op == CMP_NEQ)
            && (ir[i + 1].op == JMP_TRUE || ir[i + 1].op == JMP_FALSE)
            && ir[i].target.type == IRVT_TEMP
            && ir[i + 1].u.args[0].type == IRVT_TEMP
            && ir[i + 1].u.args[0].u.reg_id == ir[i].target.u.reg_id
            && !lifetime_check_at(info, ir[i + 2].target.u.reg_id, IRVT_TEMP, i + 2))
        {
            assert(lifetime_check_at(
                       info, ir[i].target.u.reg_id, ir[i].target.type, i + 1));

            ir_val empty_val = IR_EMPTY();
            ir_op orig_cmp_op = ir[i].op;
            ir[i].target = empty_val;
            ir[i].op = CMP_SET_FLAGS;

            ir[i + 1].op = get_correct_jump_op(orig_cmp_op, ir[i + 1].op == JMP_FALSE);
            ir[i + 1].u.args[0] = ir[i + 1].u.args[1];
        }
    }
}

static void
optimize_func(d_func* func)
{
    ir_quadr* ir = func->code;
    opt_ctx ctx = opt_create_ctx(ir);
    opt_del_dead_labels(&ir, &ctx);
    opt_fallthrough_jumps(&ir);

    // TODO: dont' recreate context, although we need it to recalculate unused labels:
    ctx = opt_create_ctx(ir);
    opt_del_dead_labels(&ir, &ctx);

    lifetime_info info = compute_lifetimes(ir);

#if 0
    mm ir_size = array_size(ir);
    printf("Optimizing func: \"%s\"\n", func->name);
    for (mm i = 0; i < ir_size; ++i)
    {
        printf("Alive: [ ");

        for (mm j = 0; j < info.n_vars; ++j)
            if (info.mem[i * info.n_all + j])
                printf("v_%ld ", j);

        for (mm j = 0; j < info.n_fparams; ++j)
            if (info.mem[i * info.n_all + info.n_vars + j])
                printf("p_%ld ", j);

        for (mm j = 0; j < info.n_temps; ++j)
            if (info.mem[i * info.n_all + info.n_vars + info.n_fparams + j])
                printf("t_%ld ", j);

        printf("]\n");
    }
#endif

    replace_compare_jumps_with_flag_jumps(&ir, &info);
    func->regalloc = allocate_registers_for_temps(ir, &info);

    func->code = ir;
    lifetime_free(&info);
}

static void
optimize()
{
    mm first_f = FUNCID_LAST_BUILTIN_FUNC + 1;
    for (mm i = first_f, size = array_size(g_funcs); i < size; ++i)
    {
        optimize_func(g_funcs + i);
    }
}
