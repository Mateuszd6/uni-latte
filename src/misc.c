// Common definitions, type aliases and utility functions

#include <stddef.h>
#include <stdint.h>

typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef float f32;
typedef double f64;
typedef size_t umm;
typedef ptrdiff_t mm;
typedef i8 b8;
typedef i32 b32;

#define MIN(LHS, RHS) (((LHS) < (RHS)) ? (LHS) : (RHS))
#define MAX(LHS, RHS) (((LHS) > (RHS)) ? (LHS) : (RHS))
#define SWAP(LHS, RHS)                                                         \
    do {                                                                       \
        __typeof(LHS) LHS_ = LHS;                                              \
        __typeof(RHS) RHS_ = RHS;                                              \
        (LHS) = RHS_;                                                          \
        (RHS) = LHS_;                                                          \
    } while(0)

#if 0
#include <stdarg.h>
static void
cerror(char* fmt, ...)
{
    va_list args;

    fflush(stdout);
    fprintf(stderr, "error: ");
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");

    exit(1);
}

static int
qsort_strcmp(void const* lhs_p, void const* rhs_p)
{
    char const* lhs = *(char const* const*)(lhs_p);
    char const* rhs = *(char const* const*)(rhs_p);

    return strcmp(lhs, rhs);
}

typedef struct ctx ctx;
struct ctx
{
    FILE* out;
    char** vars;
};

static mm
ctx_var_idx(ctx c, char* var_name)
{
    // It would be sagnificantly faster to user a fullscan here, because bsearch
    // is kind of slow, but there is a formal requirement that compilation must
    // be nlogn, so here we go:
    char** found = bsearch(&var_name, c.vars, (umm)array_size(c.vars),
                           sizeof(char*), qsort_strcmp);

    if (!found) found = c.vars; // Should not hit, unless there is a bug
    return found - c.vars;
}
#endif
