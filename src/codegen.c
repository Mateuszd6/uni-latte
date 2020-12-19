static void
generate_glob_func(ir_quadr* ir, char* fname)
{
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
        case LABEL:
        case JMP:
        case PARAM:
        case CALL:
        case RET:
        case SUBSCR:
        case ARR_LEN:
        case ALLOC:
        case STR_ADD:
        case JMP_TRUE:
        case JMP_FALSE:
        {

        } break;
        }
    }
}
