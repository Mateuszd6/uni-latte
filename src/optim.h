#ifndef OPTIM_H_
#define OPTIM_H_

#define MAX_ALLOCATED_REGS (12) // TODO: config

typedef struct opt_ctx opt_ctx;
struct opt_ctx
{
    b8* used_labels;
};

#endif // OPTIM_H_
