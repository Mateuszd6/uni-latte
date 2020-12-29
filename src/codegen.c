#include "codegen.h"

//
// This code dumps very simple IR so it's easier to debug the code. This is
// entirely skipped in the relase mode
//

#if DUMP_IR

static void
gen_ir_for_function(u32 f_id)
{
    ir_quadr* ircode = g_funcs[f_id].code;
    char* fname = g_funcs[f_id].name;

    fprintf(ir_dest, "GLOBAL_FUNC_%u ; %s\n", f_id, fname);
    for (mm i = 0, size = array_size(ircode); i < size; ++i)
    {
        if (ircode[i].op == LABEL)
        {
            // NOP
        }
        else if (ircode[i].target.type != IRVT_NONE)
        {
            fprintf(ir_dest, "    %s%ld = ",
                    ir_val_type_name[ircode[i].target.type],
                    ircode[i].target.u.reg_id); // TODO: OR CONSTANT?
        }
        else
        {
            fprintf(ir_dest, "    ");
        }

        fprintf(ir_dest, "%s", ir_op_name[ircode[i].op]);

        for (mm a = 0; a < ir_op_n_args[ircode[i].op]; ++a)
        {
            if (ircode[i].u.args[a].type == IRVT_NONE)
                fprintf(ir_dest, " _");
            else
                fprintf(ir_dest, " %s%ld",
                        ir_val_type_name[ircode[i].u.args[a].type],
                        ircode[i].u.args[a].u.reg_id);
        }

        fprintf(ir_dest, "\n");
    }

    fprintf(ir_dest, "\n");
}

static void
gen_ir()
{
    mm first_f = FUNCID_LAST_BUILTIN_FUNC + 1;
    for (mm i = first_f, size = array_size(g_funcs); i < size; ++i)
    {
        gen_ir_for_function((u32)i);
    }
}

#endif

static u8
get_reg_for(ir_val* v, codegen_ctx* ctx)
{
    switch (v->type) {
    case IRVT_VAR: return ctx->regalloc.vars[v->u.constant];
    case IRVT_FNPARAM: return ctx->regalloc.params[v->u.constant];
    case IRVT_TEMP: return ctx->regalloc.temps[v->u.constant];
    case IRVT_CONST:
    case IRVT_STRCONST:
    case IRVT_NONE:
    case IRVT_FN:
        return 0;
    }

    return 0; // NOTREACHED
}

static void
save_registers(codegen_ctx* ctx)
{
    for (mm i = 0; i < X64_NUM_REGS; ++i)
        if (ctx->used_regs[i])
            fprintf(asm_dest, "    push    %s\n", x64_reg_name[i]);
}

static void
restore_registers(codegen_ctx* ctx)
{
    for (mm i = X64_NUM_REGS - 1; i >= 0; --i)
        if (ctx->used_regs[i])
            fprintf(asm_dest, "    pop     %s\n", x64_reg_name[i]);
}

static void
gen_entry_point(i32 main_f_id)
{
    fprintf(asm_dest, PROGRAM_ENTRY_POINT, main_f_id);
}

static void
gen_func_prologue(i32 f_id, codegen_ctx* ctx, char const* fname)
{
    fprintf(asm_dest, ".GF%d: ; %s\n", f_id, fname);
    fprintf(asm_dest, "    push    rbp\n");
    fprintf(asm_dest, "    mov     rbp, rsp\n");
    if (ctx->bp_offset)
        fprintf(asm_dest, "    sub     rsp, %ld\n", ctx->bp_offset * 8);

    save_registers(ctx);

    // Load register-allocated params into apropiate regs:
    // TODO: Get nparams, don't use it like this!
    mm i = 0;
    for (u8* p = ctx->regalloc.params; p != ctx->regalloc.temps; ++p, ++i)
    {
        if (*p)
        {
            fprintf(asm_dest, "    mov     %s, QWORD [rbp+%ld] ; load p_%ld into reg\n",
                    x64_reg_name[*p], 8 * (i + 2), i);
        }
    }
}

static void
gen_func_epilogue(codegen_ctx* ctx)
{
    restore_registers(ctx);

    if (ctx->bp_offset)
        fprintf(asm_dest, "    add     rsp, %ld\n", ctx->bp_offset * 8); // TODO: Don't gen for n_locals = 0
    fprintf(asm_dest, "    pop     rbp\n");
}

static void
gen_codefor_const(char* buf, i64 constant)
{
    sprintf(buf, "%ld", constant);
}

static void
gen_codefor_other_reg(char* buf, x64_reg r)
{
    sprintf(buf, "%s", x64_reg_name[r]);
}

static void
gen_codefor_local_variable(char* buf, i64 var_id, codegen_ctx* ctx)
{
    assert(ctx->bp_vars[var_id] != 0);
    sprintf(buf, "QWORD [rbp-%d]", 8 * ctx->bp_vars[var_id]);
}

static void
gen_codefor_fun_param(char* buf, i64 var_id)
{
    sprintf(buf, "QWORD [rbp+%ld]", 8 * (var_id + 2));
}

static void
gen_codefor_temp_variable(char* buf, i64 temp_id, codegen_ctx* ctx)
{
    assert(ctx->bp_temps[temp_id] != 0);
    sprintf(buf, "QWORD [rbp-%d]", 8 * ctx->bp_temps[temp_id]);
}

static void
gen_codefor_data_segment(char* buf, char const* seg, i64 constant)
{
    sprintf(buf, "%s%ld", seg, constant);
}

static void
gen_get_address_of(char* buf, ir_val* v, codegen_ctx* ctx)
{
    switch (v->type) {
    case IRVT_VAR:
    {
        if (ctx->regalloc.vars[v->u.constant])
            gen_codefor_other_reg(buf, ctx->regalloc.vars[v->u.constant]);
        else
            gen_codefor_local_variable(buf, v->u.constant, ctx);
    } break;
    case IRVT_FNPARAM:
    {
        if (ctx->regalloc.params[v->u.constant])
            gen_codefor_other_reg(buf, ctx->regalloc.params[v->u.constant]);
        else
            gen_codefor_fun_param(buf, v->u.constant);
    } break;
    case IRVT_TEMP:
    {
        if (ctx->regalloc.temps[v->u.constant])
            gen_codefor_other_reg(buf, ctx->regalloc.temps[v->u.constant]);
        else
            gen_codefor_temp_variable(buf, v->u.constant, ctx);
    } break;
    case IRVT_CONST:
    {
        gen_codefor_const(buf, v->u.constant);
    } break;
    case IRVT_STRCONST:
    {
        // Offset to the string in the .DATA segment
        gen_codefor_data_segment(buf, ".BS", v->u.constant);
    } break;
    case IRVT_NONE:
    case IRVT_FN:
    {
        NOTREACHED; // Should not reach, TODO?
    } break;
    }
}

static void
gen_simple_op(x64_reg r, ir_val* v, char const* op, codegen_ctx* ctx)
{
    char buf[64];
    gen_get_address_of(buf, v, ctx);

    fprintf(asm_dest, "    %s     %s, %s\n", op, x64_reg_name[r], buf);
}

static void
gen_simple_op_rev(x64_reg r, ir_val* v, char const* op, codegen_ctx* ctx)
{
    char buf[64];
    gen_get_address_of(buf, v, ctx);

    fprintf(asm_dest, "    %s     %s, %s\n", op, buf, x64_reg_name[r]);
}

static void
gen_load(x64_reg r, ir_val* v, codegen_ctx* ctx)
{
    if (v->type == IRVT_CONST && v->u.constant == 0)
    {
        fprintf(asm_dest, "    xor     %s, %s\n", x64_reg_name[r], x64_reg_name[r]);
        return;
    }

    gen_simple_op(r, v, "mov", ctx);
}

static void
gen_store(ir_val* v, x64_reg r, codegen_ctx* ctx)
{
    gen_simple_op_rev(r, v, "mov", ctx);
}

// TODO: Test with negative constants
static void
gen_store_const(ir_val* v, u64 constant, codegen_ctx* ctx)
{
    char buf[64];
    gen_get_address_of(buf, v, ctx);
    fprintf(asm_dest, "    mov     %s, %lu\n", buf, constant);
}

static void
gen_mov(ir_quadr* q, codegen_ctx* ctx)
{
    u8 reg_id = 0;
    char buf[64];

    if (q->u.args[0].type == IRVT_CONST)
    {
        gen_store_const(&q->target, q->u.args[0].u.constant, ctx);
        return;
    }

    // If a target is in the register, just move to the register
    reg_id = get_reg_for(&q->target, ctx);
    if (reg_id)
    {
        gen_get_address_of(buf, q->u.args + 0, ctx);
        fprintf(asm_dest, "    mov     %s, %s\n", x64_reg_name[reg_id], buf);
        return;
    }

    // If target is not in a register, but a source is in register, x64 has a
    // mov for that too
    reg_id = get_reg_for(q->u.args + 0, ctx);
    if (reg_id)
    {
        gen_get_address_of(buf, &q->target, ctx);
        fprintf(asm_dest, "    mov     %s, %s\n", buf, x64_reg_name[reg_id]);
        return;
    }

    // There is no mov from mem to mem, so we need to use an additional register
    // for that:
    gen_load(RAX, q->u.args + 0, ctx);
    gen_store(&q->target, RAX, ctx);
}

static void
gen_arithm_bin(ir_quadr* q, char const* op, codegen_ctx* ctx)
{
    // TODO: If target is in regiser, don't use rax, just mov into target and do
    // the operation there

    gen_load(RAX, q->u.args + 0, ctx);
    gen_simple_op(RAX, q->u.args + 1, op, ctx);
    gen_store(&q->target, RAX, ctx);
}

static void
gen_add(ir_quadr* q, codegen_ctx* ctx)
{
    gen_arithm_bin(q, "add", ctx);
}

static void
gen_sub(ir_quadr* q, codegen_ctx* ctx)
{
    gen_arithm_bin(q, "sub", ctx);
}

// TODO: Use LEA for faster constant multiplication
static void
gen_mul(ir_quadr* q, codegen_ctx* ctx)
{
    gen_arithm_bin(q, "imul", ctx);
}

static void
gen_and(ir_quadr* q, codegen_ctx* ctx)
{
    gen_arithm_bin(q, "and", ctx);
}

static void
gen_or(ir_quadr* q, codegen_ctx* ctx)
{
    gen_arithm_bin(q, "or ", ctx);
}

static void
gen_not(ir_quadr* q, codegen_ctx* ctx)
{
    // TODO: Instead of loading to RDX test can be alredy performed on the
    //       allocated register if that is the case

    gen_load(RDX, q->u.args + 0, ctx);

    fprintf(asm_dest, "    xor     %s, %s\n", x64_reg_name[RAX], x64_reg_name[RAX]);
    fprintf(asm_dest, "    test    %s, %s\n", x64_reg_name[RDX], x64_reg_name[RDX]);
    fprintf(asm_dest, "    sete    al\n"); // TODO: Name of the bottom parts of regs

    // TODO: Same case: if allocated register probably not needed, need to xor
    // different reg though
    gen_store(&q->target, RAX, ctx);
}

static void
gen_div_or_mod(ir_quadr* q, b32 take_reminder, codegen_ctx* ctx)
{
    char buf[64];
    gen_get_address_of(buf, q->u.args + 1, ctx);

    gen_load(RAX, q->u.args + 0, ctx);
    fprintf(asm_dest, "    cqo\n");

    // Division by constant is handled differently, b/c idiv const does not work
    // So we use a temp variable (one-off to the stack) to store the divisor.
    // This is not fast by any streatch of imagination, but this way we regain
    // RCX, which seems to be worth it
    if (q->u.args[1].type == IRVT_CONST)
    {
        mm v = q->u.args[1].u.constant;
        fprintf(asm_dest, "    mov     QWORD [rsp-8], %ld\n", v);
        fprintf(asm_dest, "    idiv    QWORD [rsp-8]\n");
    }
    else
    {
        fprintf(asm_dest, "    idiv    %s\n", buf);
    }

    // RAX for divider, RDX for reminder:
    gen_store(&q->target, take_reminder ? RDX : RAX, ctx);
}

static void
gen_compare_flags(ir_quadr* q, codegen_ctx* ctx)
{
    // TODO: Instead of loading to RDX test can be alredy performed on the
    //       allocated register if that is the case

    // Here RDX gets loaded with the second operand, first _can_ be in memory,
    // THE ORDER MATTERS, otherwise we would have to swap the comparison
    // operator (swap != negate, so <= becomes >=)

    char buf[64];
    gen_load(RDX, q->u.args + 0, ctx);
    gen_get_address_of(buf, q->u.args + 1, ctx);
    fprintf(asm_dest, "    cmp     %s, %s\n", x64_reg_name[RDX], buf);
}

static void
gen_compare(ir_quadr* q, compar_op op, codegen_ctx* ctx)
{
    // TODO: Instead of loading to RDX test can be alredy performed on the
    //       allocated register if that is the case

    fprintf(asm_dest, "    xor     %s, %s\n", x64_reg_name[RAX], x64_reg_name[RAX]);
    gen_compare_flags(q, ctx);
    fprintf(asm_dest, "    set%s   al\n", compar_op_name[op]);

    // TODO: Name of the bottom parts of regs
    // TODO: Same case: if allocated register probably not needed, need to xor
    // different reg though
    gen_store(&q->target, RAX, ctx);
}

static void
gen_str_cmp(ir_quadr* q, compar_op op, codegen_ctx* ctx)
{
    char buf1[64];
    char buf2[64];
    gen_get_address_of(buf1, q->u.args + 0, ctx);
    gen_get_address_of(buf2, q->u.args + 1, ctx);

    fprintf(asm_dest, "    push    %s\n", buf2);
    fprintf(asm_dest, "    push    %s\n", buf1);
    fprintf(asm_dest, "    call    .BF0\n");
    fprintf(asm_dest, "    add     rsp, 16 ; cleanup\n"); // Cleanup 2 args

    // If testing for non-equality, reverse the input.
    // TODO: Use dest register instead of rax when taget is in reg
    if (op == COP_NE)
    {
        fprintf(asm_dest, "    xor     edx, edx\n");
        fprintf(asm_dest, "    test    eax, eax\n");
        fprintf(asm_dest, "    sete    dl\n");
        fprintf(asm_dest, "    mov     eax, edx\n");
    }

    // TODO: Same case: if allocated register probably not needed, need to xor
    // different reg though
    gen_store(&q->target, RAX, ctx);
}

static void
gen_str_add(ir_quadr* q, codegen_ctx* ctx)
{
    char buf1[64];
    char buf2[64];
    gen_get_address_of(buf1, q->u.args + 0, ctx);
    gen_get_address_of(buf2, q->u.args + 1, ctx);

    fprintf(asm_dest, "    push    %s\n", buf2);
    fprintf(asm_dest, "    push    %s\n", buf1);
    fprintf(asm_dest, "    call    .BF1\n");
    fprintf(asm_dest, "    add     rsp, 16 ; cleanup\n"); // Cleanup 2 args

    gen_store(&q->target, RAX, ctx);
}

static void
gen_label(ir_quadr* q)
{
    fprintf(asm_dest, ".L%ld:\n", q->u.args[0].u.constant);
}

static void
gen_jump(ir_quadr* q)
{
    fprintf(asm_dest, "    jmp     .L%ld\n", q->u.args[0].u.constant);
}

static void
gen_jump_flags(ir_quadr* q)
{
    char* op = 0;
    switch (q->op) {
    case JMP_FLAGS_E: op = "e"; break;
    case JMP_FLAGS_NE: op = "ne"; break;
    case JMP_FLAGS_L: op = "l"; break;
    case JMP_FLAGS_LE: op = "le"; break;
    case JMP_FLAGS_G: op = "g"; break;
    case JMP_FLAGS_GE: op = "ge"; break;
    default: NOTREACHED;
    }

    fprintf(asm_dest, "    j%s     .L%ld\n", op, q->u.args[0].u.constant);
}

static void
gen_conditional_jump(ir_quadr* q, b32 is_negated, codegen_ctx* ctx)
{
    // TODO: Instead of loading to RAX test can be alredy performed on the
    //       allocated register if that is the case

    gen_load(RAX, q->u.args + 0, ctx);
    fprintf(asm_dest, "    test    %s, %s\n", x64_reg_name[RAX], x64_reg_name[RAX]);
    fprintf(asm_dest, "    j%s     .L%ld\n",
            is_negated ? "e " : "ne", q->u.args[1].u.constant);

    // yes, it's JE if is_negated, becasue you test against 0, which means false
}

static void
gen_param(ir_quadr* q, codegen_ctx* ctx)
{
    char buf[64];
    gen_get_address_of(buf, q->u.args + 0, ctx);

    fprintf(asm_dest, "    push    %s\n", buf);
}

static void
gen_call(ir_quadr* q, codegen_ctx* ctx)
{
    if (q->u.args[0].type == IRVT_FN)
    {
        fprintf(asm_dest, "    call    .GF%ld\n", q->u.args[0].u.constant);

        // Cleanup if args were passed on the stack
        if (g_funcs[q->u.args[0].u.constant].num_args > 0)
        {
            fprintf(asm_dest, "    add     rsp, %d ; cleanup\n",
                    8 * g_funcs[q->u.args[0].u.constant].num_args);
        }

        // If function returns something, write it to the result
        if (q->target.type != IRVT_NONE)
            gen_store(&q->target, RAX, ctx);
    }
    else
    {
        // TODO: Something else for local functions
        NOTREACHED; // TODO
    }
}

static mm
count_locals(ir_quadr* ir)
{
    mm retval = 0;
    for (mm i = 0, size = array_size(ir); i < size; ++i)
    {
        if (ir[i].target.type == IRVT_VAR)
            retval = MAX(retval, ir[i].target.u.reg_id);
    }

    return retval + 1;
}

static void
gen_glob_func(u32 f_id)
{
    d_func* func = g_funcs + f_id;
    ir_quadr* ir = func->code;
    char* fname = func->name;
    i32 return_label_id = func->return_label_id;
    codegen_ctx ctx = {
        .regalloc = func->regalloc,
        .used_regs = {0},
        .bp_vars = func->regalloc.bp_vars,
        .bp_temps = func->regalloc.bp_temps,
        .bp_offset = func->regalloc.bp_offset,
    };

    // Set used regs
    for (mm i = 0; i < ctx.regalloc.n_all; ++i)
        if (ctx.regalloc.all[i])
            ctx.used_regs[ctx.regalloc.all[i]] = 1;

    gen_func_prologue((i32)f_id, &ctx, fname);
    for (mm i = 0, size = array_size(ir); i < size; ++i)
    {
        ir_quadr q = ir[i];
        switch(q.op) {

        case MOV:
        {
            gen_mov(&q, &ctx);
        } break;
        case ADD:
        {
            gen_add(&q, &ctx);
        } break;
        case SUB:
        {
            gen_sub(&q, &ctx);
        } break;
        case MUL:
        {
            gen_mul(&q, &ctx);
        } break;
        case DIV:
        {
            gen_div_or_mod(&q, 0, &ctx);
        } break;
        case MOD:
        {
            gen_div_or_mod(&q, 1, &ctx);
        } break;
        case AND:
        {
            gen_and(&q, &ctx);
        } break;
        case OR:
        {
            gen_or(&q, &ctx);
        } break;
        case NOT:
        {
            gen_not(&q, &ctx);
        } break;

        case CMP_LTH:
        {
            gen_compare(&q, COP_L, &ctx);
        } break;
        case CMP_LE:
        {
            gen_compare(&q, COP_LE, &ctx);
        } break;
        case CMP_GTH:
        {
            gen_compare(&q, COP_G, &ctx);
        } break;
        case CMP_GE:
        {
            gen_compare(&q, COP_GE, &ctx);
        } break;
        case CMP_EQ:
        {
            gen_compare(&q, COP_EQ, &ctx);
        } break;
        case CMP_NEQ:
        {
            gen_compare(&q, COP_NE, &ctx);
        } break;

        case STR_EQ:
        {
            gen_str_cmp(&q, COP_EQ, &ctx);
        } break;
        case STR_NEQ:
        {
            gen_str_cmp(&q, COP_NE, &ctx);
        } break;
        case STR_ADD:
        {
            gen_str_add(&q, &ctx);
        } break;

        case LABEL:
        {
            gen_label(&q);
        } break;
        case JMP:
        {
            gen_jump(&q);
        } break;
        case JMP_TRUE:
        {
            gen_conditional_jump(&q, 0, &ctx);
        } break;
        case JMP_FALSE:
        {
            gen_conditional_jump(&q, 1, &ctx);
        } break;

        case PARAM:
        {
            gen_param(&q, &ctx);
        } break;
        case CALL:
        {
            gen_call(&q, &ctx);
        } break;
        case RET:
        {
            if (ir[i].u.args[0].type != IRVT_NONE)
                gen_load(RAX, ir[i].u.args + 0, &ctx);

            if (i + 1 < size && ir[i + 1].op == LABEL && ir[i + 1].u.args[0].u.constant == return_label_id)
                printf("Yup\n");
            else
                fprintf(asm_dest, "    jmp     .L%d\n", return_label_id);
        } break;

        case CMP_SET_FLAGS:
        {
            gen_compare_flags(&q, &ctx);
        } break;

        case JMP_FLAGS_E:
        case JMP_FLAGS_NE:
        case JMP_FLAGS_L:
        case JMP_FLAGS_LE:
        case JMP_FLAGS_G:
        case JMP_FLAGS_GE:
        {
            gen_jump_flags(&q);
        } break;

        case SUBSCR:
        case ARR_LEN:
        case ALLOC:
        {
            // TODO:
        } break;

        case LOAD:
        case SPILL:
        {
            // TODO:
        } break;

        case NOP:
        case DISPOSE:
        {
            // Optimisation-related and ignored
        } break;
        }
    }

    gen_func_epilogue(&ctx);
    fprintf(asm_dest, "    ret\n");
    fprintf(asm_dest, "\n");
}

static void
gen_constants()
{
    for (mm i = 0, size = array_size(g_str_consts); i < size; ++i)
    {
        fprintf(asm_dest, ".BS%ld:\n    db \"%s\",0x0\n", i, g_str_consts[i].data);
    }
}

static void
gen_code()
{
    fwrite(gen_asm_intro, 1, COUNT_OF(gen_asm_intro) - 1, asm_dest);

    mm first_f = FUNCID_LAST_BUILTIN_FUNC + 1;
    mm main_id = 0;
    for (mm i = first_f, size = array_size(g_funcs); i < size; ++i)
    {
        gen_glob_func((u32)i);
        if (strcmp(g_funcs[i].name, "main") == 0)
            main_id = i;
    }

    gen_entry_point((i32)main_id);
    gen_constants();

    fwrite(gen_asm_epilogue, 1, COUNT_OF(gen_asm_epilogue) - 1, asm_dest);
}
