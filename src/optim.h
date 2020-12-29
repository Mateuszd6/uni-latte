#ifndef OPTIM_H_
#define OPTIM_H_

typedef struct life_interval life_interval;
struct life_interval
{
    mm id;
    mm start;
    mm end;
    int type;
};

typedef struct lifetime_info lifetime_info;
struct lifetime_info
{
    b8* mem;
    mm n_all;
    mm n_vars;
    mm n_fparams;
    mm n_temps;
};

typedef struct reg_alloc_info reg_alloc_info;
struct reg_alloc_info
{
    union
    {
        u8* vars;
        u8* all;
    };
    u8* params;
    u8* temps;
    mm n_all;
};

typedef struct opt_ctx opt_ctx;
struct opt_ctx
{
    b8* used_labels;
};

#endif // OPTIM_H_
