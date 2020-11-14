#ifndef SYMTAB_H_
#define SYMTAB_H_

#define HASHMAP_STATIC

#include "misc.h"
#include "array.h"
#include "hashmap.h"

typedef enum symbol_t symbol_t;
enum symbol_t
{
    S_VAR,
    S_FUN,
    S_CLASS,
};

typedef struct symbol symbol;
struct symbol
{
    void* ptr;
    symbol_t type;
    i32 id;
};

typedef struct d_var d_var;
struct d_var
{
    i32 lnum;
    i32 type_id;
};

typedef struct d_func d_func;
struct d_func
{
    i32 lnum;
    i32 ret_type_id;
    i32 num_args;
    // TODO: List of arguments.
};

typedef struct d_class d_class;
struct d_class
{
    i32 lnum;
};

typedef struct symbol_stack symbol_stack;
struct symbol_stack
{
    symbol* symbols;
};

HASHMAP_DECLARE(symboltab, char*, symbol_stack);
#define symboltab_insert(HM, K, V) symboltab_insert_((void**)&(HM), K, V)
#define symboltab_reserve(HM, N) symboltab_reserve_((void**)&(HM), N)

extern symboltab_kvp* g_symtab;

static symbol symbol_get(char* name);
static void symbol_push(char* name, symbol s);
static void symbol_pop(char* name);

// TODO: Possibly make other functions, like get func / get class / get var
//       that fail when not found or type missmatch

#endif // SYMTAB_H_