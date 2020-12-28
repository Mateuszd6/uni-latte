#ifndef OPTIM_H_
#define OPTIM_H_

#define MAX_ALLOCATED_REGS (11) // TODO: config

typedef struct reg_alloc_info reg_alloc_info;
struct reg_alloc_info
{
    u8* vars; // THis owns the memory
    u8* params;
    u8* temps;
};

typedef struct opt_ctx opt_ctx;
struct opt_ctx
{
    b8* used_labels;
};

#endif // OPTIM_H_
