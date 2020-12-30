#ifndef CODEGEN_H_
#define CODEGEN_H_

#include "frontend.h"

//
// The order of the registers matters. Note that our calling convention differs
// from System V ABI (and pretty much anything else, that's why user has to be
// super aware when interacting with C)
//

enum compar_op
{
    COP_EQ = 0,
    COP_NE = 1,
    COP_L = 2,
    COP_G = 3,
    COP_LE = 4,
    COP_GE = 5,
};
typedef enum compar_op compar_op;

static char const* compar_op_name[] = { "e ", "ne", "l ", "g ", "le", "ge", };

enum x64_reg
{
    // Internal - used for arithmetic operations & frame magagement
    RAX = 0,
    RDX = 1,
    RBP = 2,
    RSP = 3,

    // Allocatable, all registers are callee saved
    RCX = 4,
    RSI = 5,
    RDI = 6,
    R8 = 7,
    R9 = 8,
    R10 = 9,
    R11 = 10,
    RBX = 11,
    R12 = 12,
    R13 = 13,
    R14 = 14,
    R15 = 15,
};
typedef enum x64_reg x64_reg;
#define X64_NUM_REGS (16)

STATIC_ASSERT(MAX_ALLOCATED_REGS + 4 <= X64_NUM_REGS,
              Too_many_registers_for_allocation_MAX_is_12);

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
    "    mov     rdi, rax\n"                                                   \
    "    jmp     exit ; tailcall\n"                                            \
    "\n"

typedef struct codegen_ctx codegen_ctx;
struct codegen_ctx
{
    reg_alloc_info regalloc;
    i32* bp_vars; // Holds memory
    i32* bp_temps;
    mm bp_offset;
    b8 used_regs[X64_NUM_REGS];
};

static FILE* asm_dest;
#if DUMP_IR
static FILE* ir_dest;
#endif

#endif // CODEGEN_H_
