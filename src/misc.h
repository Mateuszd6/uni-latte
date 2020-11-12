// Common definitions, type aliases and utility functions

#ifndef MISC_H_
#define MISC_H_

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

// Let compiler know if the function wraps printf
#if (defined(__GNUC__) || defined(__clang__))
#  define PRINTF_FUNC(N, M) __attribute__ ((format (printf, N, M)))
#else
#  define PRINTF_FUNC(N, M)
#endif

#define MIN(LHS, RHS) (((LHS) < (RHS)) ? (LHS) : (RHS))
#define MAX(LHS, RHS) (((LHS) > (RHS)) ? (LHS) : (RHS))
#define SWAP(LHS, RHS)                                                         \
    do                                                                         \
    {                                                                          \
        __typeof(LHS) LHS_ = LHS;                                              \
        __typeof(RHS) RHS_ = RHS;                                              \
        (LHS) = RHS_;                                                          \
        (RHS) = LHS_;                                                          \
    } while (0)

#define LIST_FOREACH(ITER, HEAD, NEXT_FIELD)                                   \
    for (__typeof(HEAD) ITER = (HEAD);                                         \
         (ITER);                                                               \
         (ITER) = (ITER)-> NEXT_FIELD)

extern void accept_input(void);
extern void fatal(char* err);
extern void error(mm line, char* fmt, ...);

// Line numbers:
extern void* alloc_ast_node(size_t size);
extern mm get_lnum(void* ast_node);

#endif // MISC_H_
