#ifndef SYMTAB_H_
#define SYMTAB_H_

#define HASHMAP_STATIC

#include "misc.h"
#include "array.h"
#include "hashmap.h"

typedef enum symbol_t symbol_t;
enum symbol_t
{
    S_NONE, // Means there is no symbol
    S_VAR,
    S_FUN,
    S_TYPE,
};

typedef struct symbol symbol;
struct symbol
{
    symbol_t type;
    i32 id;
};

// TODO: replace d_* -> ?

typedef struct d_var d_var;
struct d_var
{
    i32 lnum;
    u32 type_id;
};

typedef struct d_func_arg d_func_arg;
struct d_func_arg
{
    char* name;
    u32 type_id;
};

typedef struct d_func d_func;
struct d_func
{
    d_func_arg* arg_type_ids;
    i32 lnum;
    u32 ret_type_id;
    i32 num_args;
};

typedef struct d_type d_type;
struct d_type
{
    i32 lnum;
    b32 is_primitive;
};

typedef struct symbol_stack symbol_stack;
struct symbol_stack
{
    symbol* symbols;
};

HASHMAP_DECLARE(symboltab, char*, symbol_stack);
#define symboltab_insert(HM, K, V) symboltab_insert_((void**)&(HM), K, V)
#define symboltab_reserve(HM, N) symboltab_reserve_((void**)&(HM), N)

#define TYPEID_NOTFOUND ((u32)-1)
#define TYPEID_VOID (0)
#define TYPEID_INT (1)
#define TYPEID_BOOL (2)
#define TYPEID_STRING (3)

#define TYPEID_MASK (((u32)1 << 31) - 1)
#define TYPEID_FLAG_ARRAY ((u32)1 << 31)

static symbol symbol_get(char* name, void* node);
static symbol get_shadowed_symbol(char* name);
static b32 symbol_push(char* name, symbol s);
static void symbol_pop(char* name);

static u32 symbol_resolve_type(char* name, b32 is_array, void* node);

// TODO: Possibly make other functions, like get func / get type / get var
//       that fail when not found or type missmatch

#endif // SYMTAB_H_
