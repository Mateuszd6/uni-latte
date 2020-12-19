
//
// The order of the registers matters. Note that our calling convention differs
// from System V ABI (and pretty much anything else, that's why user has to be
// super aware when interacting with C)
//

enum x64_reg
{
    // Internal - used for arithmetic operations & frame magagement. Will never
    // hold a variable
    RAX = 0,
    RDX = 1,
    RBP = 2,
    RSP = 3,

    // Calee saved
    RBX = 4,
    RCX = 5,
    RSI = 6,
    RDI = 7,
    R8 = 8,
    R9 = 9,

    // Caller saved
    R10 = 10,
    R11 = 11,
    R12 = 12,
    R13 = 13,
    R14 = 14,
    R15 = 15,
};
typedef enum x64_reg x64_reg;

static char const* x64_reg_name[] =
{
    "rax", "rdx", "rbp", "rsp", "rbx", "rcx", "rsi", "rdi",
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",
};

static void
gen_set_reg_const(x64_reg r, i64 constant)
{
    if (constant == 0)
        fprintf(asm_dest, "    xor     %s, %s\n", x64_reg_name[r], x64_reg_name[r]);
    else
        fprintf(asm_dest, "    mov     %s, %ld\n", x64_reg_name[r], constant);
}

static void
gen_glob_func(u32 f_id)
{
    ir_quadr* ir = g_funcs[f_id].code;
    char* fname = g_funcs[f_id].name;

    fprintf(asm_dest, ".GF%d: ; %s\n", (i32)f_id, fname);
    for (mm i = 0, size = array_size(ir); i < size; ++i)
    {
        ir_quadr q = ir[i];
        switch(q.op) {
        case MOV:
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case MOD:
        case AND:
        case OR:
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
        case CALL:
        {
            // TODO
        } break;

        case RET:
        {
            if (ir[i].u.args[0].type == IRVT_NONE) // return "void"
            {
                fprintf(asm_dest, "    ret\n");
            }
            else if (ir[i].u.args[0].type == IRVT_CONST) // return constant
            {
                gen_set_reg_const(RAX, ir[i].u.args[0].u.constant);
                fprintf(asm_dest, "    ret\n");
            }
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
