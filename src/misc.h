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

// Some commonly used compiler builtins and attributes and utility macros:

#if (defined(__GNUC__) || defined(__clang__))
#  define LIKELY(EXPR) __builtin_expect((EXPR), 1)
#  define UNLIKELY(EXPR) __builtin_expect((EXPR), 0)
#else
#  define LIKELY(EXPR) (EXPR)
#  define UNLIKELY(EXPR) (EXPR)
#endif

#if (defined(__GNUC__) || defined(__clang__))
#  define NOINLINE __attribute__((noinline))
#elif (defined(_MSC_VER))
#  define NOINLINE __declspec(noinline)
#else
#  define NOINLINE
#endif

#if (__STDC_VERSION__ >= 201112L)
#  define NORETURN _Noreturn
#elif (defined(__clang__)) // clang wants us to use this even if no c11
#  define NORETURN _Noreturn
#else
#  define NORETURN
#endif

#ifdef DEBUG
#    define NOTREACHED assert(!(mm)(void*)"Unreachable code reached!")
#else
#  if (defined(__GNUC__) || defined(__clang__))
#    define NOTREACHED __builtin_unreachable()
#  elif (defined(_MSC_VER))
#    define NOTREACHED __assume(0)
#  else
#    define NOTREACHED ((void)0)
#  endif
#endif

#define DEFAULT_NOTREACHED default: { NOTREACHED; } break

// Let compiler know if the function wraps printf
#if (defined(__GNUC__) || defined(__clang__))
#  define PRINTF_FUNC(N, M) __attribute__ ((format (printf, N, M)))
#else
#  define PRINTF_FUNC(N, M)
#endif

#define COUNT_OF(ARRAY) ((mm)(sizeof(ARRAY) / sizeof(ARRAY[0])))

#define STATIC_ASSERT(COND, MSG) typedef char static_assertion_##MSG[(COND)?1:-1]

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
    for (__typeof((HEAD) . NEXT_FIELD) ITER = (HEAD) . NEXT_FIELD;             \
         (ITER);                                                               \
         (ITER) = (ITER) -> NEXT_FIELD)

// Error handling:

extern void accept_input(void);
extern void fatal(char* fmt, ...);
extern void error(mm line, char* fmt, ...);
extern void note(mm line, char* fmt, ...);
extern void no_recover(void);

// Line numbers & filename:

extern char* myfilename; // There is always one file in a single run
extern void* alloc_ast_node(size_t size);
extern i32 get_lnum(void* ast_node);

extern int overflows_32bit(mm val);

#endif // MISC_H_
