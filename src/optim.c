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
#endif

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
opt_fallthrough_jumps(ir_quadr** ir, opt_ctx* ctx)
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

static void
optimize_func(d_func* func)
{
    ir_quadr* ir = func->code;
    opt_ctx ctx = opt_create_ctx(ir);
    opt_del_dead_labels(&ir, &ctx);
    opt_fallthrough_jumps(&ir, &ctx);

    // TODO: dont' recreate context, although we need it to recalculate unused labels:
    ctx = opt_create_ctx(ir);
    opt_del_dead_labels(&ir, &ctx);

    func->code = ir;
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
