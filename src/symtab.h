#ifndef SYMTAB_H_
#define SYMTAB_H_

#define HASHMAP_STATIC

#include "misc.h"

enum symbol_t
{
    S_NONE, // Means there is no symbol
    S_VAR,
    S_FUN,
    S_TYPE,
};
typedef enum symbol_t symbol_t;

typedef struct symbol symbol;
struct symbol
{
    symbol_t type;
    i32 id;
};

typedef struct d_var d_var;
struct d_var
{
    i32 lnum;
    u32 type_id;
    i32 block_id; // -1 means var in the class body
};

typedef struct d_class_mem d_class_mem;
struct d_class_mem
{
    char* name;
    i32 offset; // Offset from the start of the struct in qwords, first has 0
    u32 type_id;
    i32 lnum;
};

typedef struct d_func_arg d_func_arg;
struct d_func_arg
{
    char* name;
    u32 type_id;
    i32 lnum;
};

typedef struct d_func d_func;
struct d_func
{
    char* name;
    d_func_arg* args;
    i32 lnum;
    u32 ret_type_id;
    i32 num_args;
    b32 is_local;
};

typedef struct d_type d_type;
struct d_type
{
    char* name;
    d_class_mem* members;
    d_func* member_funcs;
    i32 lnum;
    b32 is_primitive;
};

typedef struct symbol_stack symbol_stack;
struct symbol_stack
{
    symbol* symbols;
};

typedef struct string_const string_const;
struct string_const
{
    char* data;
    mm len;
};

typedef struct parsed_type parsed_type;
struct parsed_type
{
    char* name;
    b32 is_array;
};

HASHMAP_DECLARE(symboltab, char*, symbol_stack);
#define symboltab_insert(HM, K, V) symboltab_insert_((void**)&(HM), K, V)
#define symboltab_reserve(HM, N) symboltab_reserve_((void**)&(HM), N)

#define TYPEID_NOTFOUND ((u32)-1)
#define TYPEID_VOID (0)
#define TYPEID_INT (1)
#define TYPEID_BOOL (2)
#define TYPEID_STRING (3)
#define TYPEID_NULL (4)

#define TYPEID_LAST_BUILTIN_TYPE TYPEID_NULL

#define TYPEID_MASK (((u32)1 << 31) - 1)
#define TYPEID_FLAG_ARRAY ((u32)1 << 31)

#define TYPEID_UNMASK(TID) (TID & (~TYPEID_FLAG_ARRAY))

#define FUNCID_NOTFOUND ((u32)-1)
#define FUNCID_LAST_BUILTIN_FUNC (5)

#define VARID_NOTFOUND ((u32)-1)

#define EMPTY_STRING_CONSTANT_ID (0)

static inline symbol symbol_check(char* name);
static symbol symbol_get(char* name, void* node, b32 report_error);
static symbol symbol_get_shadowed(char* name);
static b32 symbol_push(char* name, symbol s);
static void symbol_pop(char* name);

static u32 symbol_resolve_type(char* name, b32 is_array, void* node);
static u32 symbol_resolve_func(char* name, void* node);
static u32 symbol_resolve_var(char* name, void* node);

static void define_primitives(void);

static inline int
qsort_d_class_mem(void const* lhs_p, void const* rhs_p)
{
    d_class_mem const* lhs = (d_class_mem const*)(lhs_p);
    d_class_mem const* rhs = (d_class_mem const*)(rhs_p);

    return strcmp(lhs->name, rhs->name);
}

static inline int
qsort_d_func(void const* lhs_p, void const* rhs_p)
{
    d_func const* lhs = (d_func const*)(lhs_p);
    d_func const* rhs = (d_func const*)(rhs_p);

    return strcmp(lhs->name, rhs->name);
}

#endif // SYMTAB_H_
