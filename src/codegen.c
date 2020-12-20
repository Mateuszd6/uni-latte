
//
// The order of the registers matters. Note that our calling convention differs
// from System V ABI (and pretty much anything else, that's why user has to be
// super aware when interacting with C)
//

enum x64_reg
{
    // Internal - used for arithmetic operations & frame magagement
    RAX = 0,
    RDX = 1,
    RBP = 2,
    RSP = 3,

    // Allocatable, caller saved
    RCX = 4,
    RSI = 5,
    RDI = 6,
    R8 = 7,
    R9 = 8,
    R10 = 9,
    R11 = 10,

    // Allocatable, calee saved
    RBX = 11,
    R12 = 12,
    R13 = 13,
    R14 = 14,
    R15 = 15,
};
typedef enum x64_reg x64_reg;

static char const* x64_reg_name[] =
{
    "rax", "rdx", "rbp", "rsp",
    "rcx", "rsi", "rdi", "r8",
    "r9", "r10", "r11", "rbx",
    "r12", "r13", "r14", "r15",
};

#define PROGRAM_ENTRY_POINT                                                    \
    "_start:\n"                                                                \
    "    call    .GF%d ; main\n"                                               \
    "    mov     rdi, rax ; TODO: Should be 0 (or xor), but just for testing\n" \
    "    jmp     exit ; tailcall\n"

static void
gen_entry_point(i32 main_f_id)
{
    fprintf(asm_dest, PROGRAM_ENTRY_POINT, main_f_id);
}

static void
gen_func_prologue(i32 f_id, i32 n_locals, char const* fname)
{
    fprintf(asm_dest, ".GF%d: ; %s\n", f_id, fname);
    fprintf(asm_dest, "    push    rbp\n");
    fprintf(asm_dest, "    mov     rbp, rsp\n");
    fprintf(asm_dest, "    sub     rsp, %d\n", n_locals * 8); // TODO: Don't gen for n_locals = 0
    fprintf(asm_dest, "    sub     rsp, %d ; TODO: TEMP\n", 1024);
}

static void
gen_func_epilogue(i32 n_locals)
{
    fprintf(asm_dest, "    add     rsp, %d ; TODO: TEMP\n", 1024);
    fprintf(asm_dest, "    add     rsp, %d\n", n_locals * 8); // TODO: Don't gen for n_locals = 0
    fprintf(asm_dest, "    pop     rbp\n");
}

//
// TODO: Evaluate
//

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
gen_codefor_local_variable(char* buf, i64 var_id)
{
    sprintf(buf, "[rbp-%ld]", 8 * (var_id + 1));
}

static void
gen_codefor_temp_variable(char* buf, i64 temp_id, i32 n_locals)
{
    sprintf(buf, "[rbp-%ld]", 8 * (n_locals + temp_id));
}

static void
gen_get_address_of(char* buf, ir_val* v, i32 n_locals)
{
    switch (v->type) {
    case IRVT_VAR:
    {
        gen_codefor_local_variable(buf, v->u.constant);
    } break;
    case IRVT_TEMP:
    {
        gen_codefor_temp_variable(buf, v->u.constant, n_locals);
    } break;
    case IRVT_CONST:
    {
        gen_codefor_const(buf, v->u.constant);
    } break;
    case IRVT_STRCONST:
    {
        NOTREACHED; // TODO
    } break;
    case IRVT_NONE:
    case IRVT_FNPARAM:
    case IRVT_FN:
    {
        NOTREACHED; // Should not reach, TODO?
    } break;
    }
}

//
// TODO: endof: Evaluate
//

static void
gen_simple_op(x64_reg r, ir_val* v, char const* op, i32 n_locals) // TODO: replace n_locals with ctx
{
    char buf[64];
    gen_get_address_of(buf, v, n_locals);

    fprintf(asm_dest, "    %s     %s, %s\n", op, x64_reg_name[r], buf);
}

static void
gen_simple_op_rev(x64_reg r, ir_val* v, char const* op, i32 n_locals) // TODO: replace n_locals with ctx
{
    char buf[64];
    gen_get_address_of(buf, v, n_locals);

    fprintf(asm_dest, "    %s     %s, %s\n", op, buf, x64_reg_name[r]);
}

static void
gen_load(x64_reg r, ir_val* v, i32 n_locals) // TODO: replace n_locals with ctx
{
    if (v->type == IRVT_CONST && v->u.constant == 0)
    {
        fprintf(asm_dest, "    xor     %s, %s\n", x64_reg_name[r], x64_reg_name[r]);
        return;
    }

    gen_simple_op(r, v, "mov", n_locals);
}

static void
gen_store(ir_val* v, x64_reg r, i32 n_locals) // TODO: replace n_locals with ctx
{
    gen_simple_op_rev(r, v, "mov", n_locals);
}

// TODO: Test with negative constants
static void
gen_store_const(ir_val* v, u64 constant, i32 n_locals)
{
    char buf[64];
    gen_get_address_of(buf, v, n_locals);
    fprintf(asm_dest, "    mov     QWORD %s, %lu\n", buf, constant);
}

static void
gen_mov(ir_quadr* q, i32 n_locals)
{
    if (q->u.args[0].type == IRVT_CONST)
    {
        gen_store_const(&q->target, (u64)q->u.args[0].u.constant, n_locals);
        return;
    }

    // TODO: Better code if arg is constant
    // TODO: Ommit RAX register if variable is already allocated in another

    // TODO: This code should only work in case a variable is on the stack
    gen_load(RAX, q->u.args + 0, n_locals);
    gen_store(&q->target, RAX, n_locals);
}

static void
gen_arithm_bin(ir_quadr* q, char const* op, i32 n_locals)
{
    gen_load(RAX, q->u.args + 0, n_locals);
    gen_simple_op(RAX, q->u.args + 1, op, n_locals);
    gen_store(&q->target, RAX, n_locals);
}

static void
gen_add(ir_quadr* q, i32 n_locals)
{
    gen_arithm_bin(q, "add", n_locals);
}

static void
gen_sub(ir_quadr* q, i32 n_locals)
{
    gen_arithm_bin(q, "sub", n_locals);
}

static void
gen_mul(ir_quadr* q, i32 n_locals)
{
    gen_arithm_bin(q, "imul", n_locals);
}

static void
gen_div_or_mul(ir_quadr* q, b32 take_reminder, i32 n_locals)
{
    char buf[64];
    gen_get_address_of(buf, q->u.args + 1, n_locals);

    gen_load(RAX, q->u.args + 0, n_locals);
    fprintf(asm_dest, "    cqo\n");
    fprintf(asm_dest, "    idiv    QWORD %s\n", buf);

    // RAX for divider, RDX for reminder:
    gen_store(&q->target, take_reminder ? RDX : RAX, n_locals);
}

static void
gen_and(ir_quadr* q, i32 n_locals)
{
    gen_arithm_bin(q, "and", n_locals);
}

static void
gen_or(ir_quadr* q, i32 n_locals)
{
    gen_arithm_bin(q, "or ", n_locals);
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
    ir_quadr* ir = g_funcs[f_id].code;
    char* fname = g_funcs[f_id].name;
    // i32 n_args = g_funcs[f_id].num_args;
    mm n_locals = count_locals(ir);
    gen_func_prologue((i32)f_id, (i32)n_locals, fname);

    for (mm i = 0, size = array_size(ir); i < size; ++i)
    {
        ir_quadr q = ir[i];
        switch(q.op) {
        case MOV:
        {
            gen_mov(&q, (i32)n_locals);
        } break;
        case ADD:
        {
            gen_add(&q, (i32)n_locals);
        } break;
        case SUB:
        {
            gen_sub(&q, (i32)n_locals);
        } break;
        case MUL:
        {
            gen_mul(&q, (i32)n_locals);
        } break;
        case DIV:
        {
            gen_div_or_mul(&q, 0, (i32)n_locals);
        } break;
        case MOD:
        {
            gen_div_or_mul(&q, 1, (i32)n_locals);
        } break;
        case AND:
        {
            gen_and(&q, (i32)n_locals);
        } break;
        case OR:
        {
            gen_or(&q, (i32)n_locals);
        } break;
        case NOT:
        case CMP_LTH:
        case CMP_LE:
        case CMP_GTH:
        case CMP_GE:
        case CMP_EQ:
        case CMP_NEQ:
        case STR_EQ:
        case STR_NEQ:
        {
            // TODO
        } break;

        case LABEL:
        {
            fprintf(asm_dest, ".L%ld:\n", ir[i].u.args[0].u.constant);
        } break;
        case JMP:
        {
            fprintf(asm_dest, "    jmp     .L%ld\n", ir[i].u.args[0].u.constant);
        } break;
        case PARAM:
        {
            if (ir[i].u.args[0].type == IRVT_CONST)
            {
                fprintf(asm_dest, "    push    %ld\n", ir[i].u.args[0].u.constant);
            }
            else
            {
                // TODO
            }
        } break;
        case CALL:
        {
            if (ir[i].u.args[0].type == IRVT_FN)
            {
                fprintf(asm_dest, "    call    .GF%ld\n", ir[i].u.args[0].u.constant);
            }
            else
            {
                // TODO
            }
        } break;
        case RET:
        {
            gen_load(RAX, ir[i].u.args + 0, (i32)n_locals);

            gen_func_epilogue((i32)n_locals);
            fprintf(asm_dest, "    ret\n");
        } break;

        case SUBSCR:
        case ARR_LEN:
        case ALLOC:
        case STR_ADD:
        case JMP_TRUE:
        case JMP_FALSE:
        {
            // TODO
        } break;
        }
    }

    fprintf(asm_dest, "\n");
}
