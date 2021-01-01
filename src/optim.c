#include "codegen.h"
#include "frontend.h"
#include "optim.h"

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
    case IRVT_VAR: return *(row + reg_id);
    case IRVT_FNPARAM: return *(row + info->n_vars + reg_id);
    case IRVT_TEMP: return *(row + info->n_vars + info->n_fparams + reg_id);
    default: NOTREACHED;
    }

    NOTREACHED;
}

static inline void
compute_max_lifetimes(mm* n_vars, mm* n_fparams, mm* n_temps, ir_val v)
{
    switch (v.type) {
    case IRVT_VAR:
    {
        *n_vars = MAX(*n_vars, v.u.reg_id + 1);
    } break;
    case IRVT_FNPARAM:
    {
        *n_fparams = MAX(*n_fparams, v.u.reg_id + 1);
    } break;
    case IRVT_TEMP:
    {
        *n_temps = MAX(*n_temps, v.u.reg_id + 1);
    } break;
    case IRVT_NONE:
    case IRVT_FN:
    case IRVT_CONST:
    case IRVT_STRCONST:
    {
    } break;
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
        compute_max_lifetimes(&n_vars, &n_fparams, &n_temps, ir[i].target);

        mm n_args = ir_op_n_args[ir[i].op];
        if (n_args >= 1) compute_max_lifetimes(&n_vars, &n_fparams, &n_temps, ir[i].u.args[0]);
        if (n_args >= 2) compute_max_lifetimes(&n_vars, &n_fparams, &n_temps, ir[i].u.args[1]);
    }

#define SET_VAR(VAL, USED) *(row + (VAL).u.reg_id) = (USED)
#define SET_FNPARAM(VAL, USED) *(row + n_vars + (VAL).u.reg_id) = (USED)
#define SET_TEMP(VAL, USED) *(row + n_vars + n_fparams + (VAL).u.reg_id) = (USED)
#define SET_USED_OR_UNUSED(VAL, USED)                                          \
    do {                                                                       \
        switch ((VAL).type) {                                                  \
        case IRVT_VAR: SET_VAR(VAL, USED); break;                              \
        case IRVT_FNPARAM: SET_FNPARAM(VAL, USED); break;                      \
        case IRVT_TEMP: SET_TEMP(VAL, USED); break;                            \
        case IRVT_NONE:                                                        \
        case IRVT_FN:                                                          \
        case IRVT_CONST:                                                       \
        case IRVT_STRCONST:                                                    \
            break;                                                             \
        }                                                                      \
    } while (0)

    mm n_all = n_vars + n_fparams + n_temps;
    b8* all = calloc(n_all * ir_size, sizeof(b8));
    for (mm i = ir_size - 1; i >= 0; --i)
    {
        if (i < ir_size - 1)
            memcpy((all + n_all * i), (all + n_all * (i + 1)), sizeof(b8) * n_all);
        b8* row = all + (n_all * i);

        SET_USED_OR_UNUSED(ir[i].target, 0);

        mm n_args = ir_op_n_args[ir[i].op];
        if (n_args >= 1) SET_USED_OR_UNUSED(ir[i].u.args[0], 1);
        if (n_args >= 2) SET_USED_OR_UNUSED(ir[i].u.args[1], 1);
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

static inline void
compute_life_intrv(lifetime_info* info, mm ir_size, mm num, ir_val_type type,
                   mm* /* OUT */ life_starts, mm* /* OUT */ life_ends)
{
    *life_starts = 0;
    *life_ends = 0;

    mm ino = 0;
    while (ino < ir_size && !lifetime_check_at(info, num, type, ino))
        ++ino;
    *life_starts = ino;

    b32 set = 0;
    while (ino < ir_size)
    {
        if (lifetime_check_at(info, num, type, ino))
        {
            *life_ends = ino + 1;
            set = 1;
        }

        ++ino;
    }

    if (!set)
        *life_ends = ino;
}

// Can't add/remove from the array, only modify!!
static b32
remove_dead_temps(ir_quadr* ir, lifetime_info* info)
{
    printf("Doing single removing iteration!\n");

    mm ir_size = array_size(ir);
    b32 removed = 0;
    b8 dead[info->n_temps + 1]; // +1 because zero-length vlas are UB

    // Set dead variables
    memset(dead, 0, sizeof(b8) * (info->n_temps + 1));
    for (mm i = 0; i < info->n_temps; ++i)
    {
        mm life_starts;
        mm life_ends;
        compute_life_intrv(info, ir_size, i, IRVT_TEMP, &life_starts, &life_ends);

        if (life_starts == life_ends && life_ends == ir_size)
            dead[i] = 1;
    }

    // Replace dead targets with empty expr
    for (mm i = 0; i < ir_size; ++i)
    {
        if (ir[i].target.type == IRVT_TEMP && dead[ir[i].target.u.reg_id])
        {
            removed = 1;

            // Other side-effect operations that have a target must be placed here
            if (ir[i].op == CALL || ir[i].op == VIRTCALL)
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

    return removed;
}

static reg_alloc_info
allocate_registers(ir_quadr* ir, lifetime_info* info)
{
    u8* alloc_info = calloc(info->n_all, sizeof(u8)); // 0 means no register
    mm ir_size = array_size(ir);
    life_interval* intervs = 0;

    struct
    {
        mm num;
        ir_val_type type;
    } types[] = {
        { .num = info->n_vars, .type = IRVT_VAR },
        { .num = info->n_fparams, .type = IRVT_FNPARAM },
        { .num = info->n_temps, .type = IRVT_TEMP },
    };

    for (mm j = 0; j < COUNT_OF(types); ++j)
        for (mm i = 0; i < types[j].num; ++i)
        {
            mm life_starts;
            mm life_ends;
            compute_life_intrv(info, ir_size, i, types[j].type, &life_starts, &life_ends);
            if (!(life_starts == life_ends && life_ends == ir_size))
            {
                life_interval in = {
                    .id = i,
                    .start = life_starts,
                    .end = life_ends,
                    .type = types[j].type,
                };

                array_push(intervs, in);
            }
        }

    mm intervs_size = array_size(intervs);
    if (intervs_size > 0)
        qsort(intervs, intervs_size, sizeof(life_interval), qsort_life_interval);

    life_interval* regs[X64_NUM_REGS] = {0};
    unallocated_var* unalloced = 0;
    mm first_al = RCX;
    mm regs_size = first_al + MAX_ALLOCATED_REGS;
    for (mm i = 0, size = intervs_size; i < size; ++i)
    {
        life_interval life = intervs[i];
        mm r = first_al;
        for (; r < regs_size; ++r)
        {
            // I MAY be POSSIBLE to replace end < life.start with end <= life.start, but
            // this will have some implications in the codegen module and at this stage,
            // it is really hard to decide, whether or not this will break something.
            mm n_interv_alloced = array_size(regs[r]);
            if (n_interv_alloced == 0 || regs[r][n_interv_alloced - 1].end < life.start)
            {
                array_push(regs[r], life);
                break;
            }
        }

        if (r == regs_size)
        {
            unallocated_var uv = { .id = life.id, .type = life.type, };
            array_push(unalloced, uv);
        }

        printf("%s_%ld: [%ld; %ld]\n",
               life.type == IRVT_VAR ? "v" : (life.type == IRVT_FNPARAM ? "p" : "t"),
               life.id, life.start, life.end);
    }

    printf("\nREGISTER ALLOCATION:\n");
    for (mm r = first_al; r < regs_size; ++r)
    {
        printf("%s:\n", x64_reg_name[r]);
        for (mm j = 0; j < array_size(regs[r]); ++j)
        {
            switch (regs[r][j].type) {
            case IRVT_VAR: alloc_info[regs[r][j].id] = (u8)r; break;
            case IRVT_FNPARAM: alloc_info[info->n_vars + regs[r][j].id] = (u8)r; break;
            case IRVT_TEMP: alloc_info[info->n_vars + info->n_fparams + regs[r][j].id] = (u8)r; break;
            default: NOTREACHED;
            }

            printf("    %s_%ld: [%ld; %ld]\n",
                   regs[r][j].type == IRVT_VAR ? "v" : (regs[r][j].type == IRVT_FNPARAM ? "p" : "t"),
                   regs[r][j].id, regs[r][j].start, regs[r][j].end);
        }

        printf("\n");
    }

#if 1
    printf("UNALLOCATED:\n");
    for (mm i = 0, n_unalloced = array_size(unalloced); i < n_unalloced; ++i)
    {
        printf("    %s_%ld\n",
               unalloced[i].type == IRVT_VAR ? "v" : (unalloced[i].type == IRVT_FNPARAM ? "p" : "t"),
               unalloced[i].id);
    }
    printf("\n");
#endif

    mm n_unalloced = array_size(unalloced);
    i32* bp_offset = malloc(sizeof(i32) *  info->n_all);
    memset(bp_offset, 0, sizeof(i32) *  info->n_all); // 0 means non-stack

    i32 offset = 1;
    i32* bp_vars = bp_offset;
    i32* bp_temps = bp_offset + info->n_vars + info->n_fparams;

    for (mm i = 0; i < n_unalloced; ++i)
        if (unalloced[i].type == IRVT_VAR)
        {
            printf("Allocating v_%ld at rbp+%d\n", unalloced[i].id, offset * 8);
            bp_vars[unalloced[i].id] = offset++;
        }

    for (mm i = 0; i < n_unalloced; ++i)
        if (unalloced[i].type == IRVT_TEMP)
        {
            printf("Allocating t_%ld at rbp+%d\n", unalloced[i].id, offset * 8);
            bp_temps[unalloced[i].id] = offset++;
        }

    for (mm i = 0; i < info->n_vars; ++i)
        printf("%d ", bp_vars[i]);
    printf("\n");

    for (mm i = 0; i < info->n_temps; ++i)
        printf("%d ", bp_temps[i]);
    printf("\n");

    reg_alloc_info retval = {
        .vars = alloc_info,
        .params = alloc_info + info->n_vars,
        .temps = alloc_info + info->n_vars + info->n_fparams,
        .n_vars = info->n_vars,
        .n_fparams = info->n_fparams,
        .n_temps = info->n_temps,
        .bp_vars = bp_vars,
        .bp_temps = bp_temps,
        .bp_offset = offset - 1,
    };

    array_free(intervs);
    return retval;
}

static b8*
get_used_labels(ir_quadr* ir)
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

    return used_labels;
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
            if (cur_ir[i + 1].u.args[0].u.constant == cur_ir[i + 2].u.args[0].u.constant)
            {
                // This happens rarely, usually with some statement-expressions that require
                // jumping code. Anyway, since both jumps take us to the same label, we can
                // skip testing against it. Usless temporary evaluations will be removed later

                printf("Being fancy here ^2\n");
                array_push(new_ir, cur_ir[i + 2]);
            }
            else
            {
                printf("Being fancy here!\n");
                ir_quadr cjmp = cur_ir[i];
                cjmp.op = cjmp.op == JMP_TRUE ? JMP_FALSE : JMP_TRUE;
                cjmp.u.args[1].u.constant = cur_ir[i + 1].u.args[0].u.constant;

                array_push(new_ir, cjmp);
                array_push(new_ir, cur_ir[i + 2]); // Push label, because it does not change
            }

            i += 2;
            continue;
        }

        if (LIKELY(i + 1 < cur_ir_size)
            && (cur_ir[i].op == JMP)
            && cur_ir[i + 1].op == LABEL
            && cur_ir[i].u.args[0].u.constant == cur_ir[i + 1].u.args[0].u.constant)
        {
            // Simply, don't emit the jump, because we don't need it
            printf("Fixing stupid things!\n");
            continue;
        }

        array_push(new_ir, cur_ir[i]);
    }

    array_free(cur_ir);
    *ir = new_ir;
}

// Remove labels that nobody ever jumps to
static void
opt_del_dead_labels(ir_quadr** ir, b8* used_labels)
{
    ir_quadr* new_ir = 0;
    ir_quadr* cur_ir = *ir;
    mm cur_ir_size = array_size(cur_ir);
    array_reserve(new_ir, cur_ir_size);

    for (mm i = 0; i < cur_ir_size; ++i)
    {
        if (cur_ir[i].op == LABEL
            && !used_labels[cur_ir[i].u.args[0].u.constant]
            && i + 1 < cur_ir_size) // We don't want to remove last label
        {
            continue;
        }

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
#ifdef OPTIMIZE
    b8* used_labels = get_used_labels(ir);
    opt_del_dead_labels(&ir, used_labels);
    opt_fallthrough_jumps(&ir);

    free(used_labels);
    used_labels = get_used_labels(ir);
    opt_del_dead_labels(&ir, used_labels);
#endif

    lifetime_info info = compute_lifetimes(ir);

#ifdef OPTIMIZE
    replace_compare_jumps_with_flag_jumps(&ir, &info);

    // Keep removing unused temp registers:
    for (;;)
    {
        b32 r = remove_dead_temps(ir, &info);
        if (!r) break;

        // If we removed something, we need to recalculate variable lifetimes:
        lifetime_free(&info);
        info = compute_lifetimes(ir);
    }
#endif

    func->regalloc = allocate_registers(ir, &info);
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
