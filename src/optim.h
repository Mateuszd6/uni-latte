#ifndef OPTIM_H_
#define OPTIM_H_

typedef struct unallocated_var unallocated_var;
struct unallocated_var
{
    mm id;
    int type;
};

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
    u8* vars; // Holds memory
    u8* params;
    u8* temps;
    mm n_vars;
    mm n_fparams;
    mm n_temps;

    i32* bp_vars; // Holds memory
    i32* bp_temps;
    mm bp_offset;
};

#endif // OPTIM_H_
