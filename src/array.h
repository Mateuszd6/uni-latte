//
// A generic C expandable array (like C++'s vector) written by me long time ago,
// heavily inspired by stb_ds: github.com/nothings/stb/blob/master/stb_ds.h .
//

#ifndef ARRAY_H_
#define ARRAY_H_

#include <stddef.h> // size_t/ptrdiff_t

#ifndef ARRAY_ASSERT
#include <assert.h>
#define ARRAY_ASSERT assert
#endif // ARRAY_ASSERT

#ifndef ARRAY_NOERRNO
#include <errno.h>
#define ARRAY_ENOMEM ENOMEM
#else
#define ARRAY_ENOMEM 1 // No errno so any non zero value means error.
#endif                 // ARRAY_NOERRNO

#if !defined(ARRAY_REALLOC) && !defined(ARRAY_FREE)
#include <stdlib.h>
#define ARRAY_REALLOC realloc
#define ARRAY_FREE free
#else
#if !defined(ARRAY_REALLOC) || !defined(ARRAY_FREE)
#error You must define but realloc and free or none of them.
#endif
#endif // !defined(ARRAY_REALLOC) && !defined(ARRAY_FREE)

#ifndef ARRAY_MEMCPY
#include <string.h>
#define ARRAY_MEMCPY memcpy
#endif // ARRAY_MEMCPY

#ifndef ARRAY_MIN_CAPACITY
#define ARRAY_MIN_CAPACITY (16)
#endif // ARRAY_MIN_CAPACITY

// static assert that ARRAY_MIN_CAPACITY is divisible by 2.
typedef char array_default_cap_assert[(ARRAY_MIN_CAPACITY % 2 == 0) ? 1 : -1];

#ifdef ARRAY_STATIC
#define ARRAY_FUNC_DECORATOR static
#else
#ifdef __cplusplus
#define ARRAY_FUNC_DECORATOR extern "C"
#else
#define ARRAY_FUNC_DECORATOR extern
#endif
#endif // ARRAY_STATIC

#if (defined(__GNUC__) || defined(__clang__))
#define ARRAY_LIKELY(EXPR) __builtin_expect((EXPR), 1)
#define ARRAY_UNLIKELY(EXPR) __builtin_expect((EXPR), 0)
#else
#define ARRAY_LIKELY(EXPR) (EXPR)
#define ARRAY_UNLIKELY(EXPR) (EXPR)
#endif // (defined(__GNUC__) || defined(__clang__))

typedef struct array_header array_header;
struct array_header
{
    ptrdiff_t capacity;
    ptrdiff_t size;
};

ARRAY_FUNC_DECORATOR inline array_header*
array_dtl_get_header(void* array)
{
    array_header* head = NULL;
    if (array)
        head = (array_header*)(void*)(((char*)array) - sizeof(array_header));

    return head;
}

ARRAY_FUNC_DECORATOR inline ptrdiff_t
array_dtl_get_size(void* array)
{
    array_header* head = array_dtl_get_header(array);
    return head ? head->size : 0;
}

ARRAY_FUNC_DECORATOR inline ptrdiff_t
array_dtl_get_capacity(void* array)
{
    array_header* head = array_dtl_get_header(array);
    return head ? head->capacity & (~1) : 0;
}

ARRAY_FUNC_DECORATOR inline int
array_dtl_need_resize(void* array, ptrdiff_t n)
{
    array_header* head = array_dtl_get_header(array);
    return head ? (head->size + n > (head->capacity & (~1))) : 1;
}

ARRAY_FUNC_DECORATOR inline int
array_dtl_is_inline(void* array)
{
    array_header* head = array_dtl_get_header(array);
    return head ? (int)(head->capacity & 1) : 0;
}

// Performs a bunch of asserts checking if everything _seems_ to be allright
// with the array.
ARRAY_FUNC_DECORATOR inline void
array_dtl_check(void* array)
{
    ARRAY_ASSERT((ARRAY_MIN_CAPACITY) % 2 == 0 &&
                 "Default capacity must be divisible by 2!");

    if (array)
    {
        ARRAY_ASSERT(array_dtl_get_capacity(array) >=
                     array_dtl_get_size(array));
    }
}

ARRAY_FUNC_DECORATOR inline ptrdiff_t
array_dtl_get_size_and_incr(void* array, ptrdiff_t n)
{
    array_header* header_ptr = array_dtl_get_header(array);
    ptrdiff_t retval = header_ptr->size;
    header_ptr->size += n;
    ARRAY_ASSERT(header_ptr->size <= header_ptr->capacity);

    return retval;
}

ARRAY_FUNC_DECORATOR inline ptrdiff_t
array_dtl_decr_and_get_size(void* array, ptrdiff_t n)
{
    array_header* header_ptr = array_dtl_get_header(array);
    header_ptr->size -= n;

    return header_ptr->size;
}

ARRAY_FUNC_DECORATOR inline void
array_dtl_clear(array_header* head)
{
    head->size = 0;
}

ARRAY_FUNC_DECORATOR inline void*
array_dtl_inline_init(void* mem, ptrdiff_t capacity)
{
    // Always round capacity up to the number divisable by 2.
    capacity = (capacity + 1) & (~1);

    array_header* header_ptr = (array_header*)mem;
    header_ptr->size = 0;
    header_ptr->capacity = capacity | 1;

    return ((char*)header_ptr) + sizeof(array_header);
}

// Enforce that the capacity is at least dest_size. Will trigger realloc if
// needed. Retunrs non-zero on failure.
ARRAY_FUNC_DECORATOR int
array_dtl_reserve(void** array_ptr, ptrdiff_t dest_size, ptrdiff_t item_size);

// This assumes array is not NULL and array is NOT STACK allocated! Fit array if
// size is more than twice lower than the capacity.
ARRAY_FUNC_DECORATOR int
array_dtl_fit(void** array_ptr, ptrdiff_t item_size);

#define array_size(ARRAY) (array_dtl_check(ARRAY), array_dtl_get_size(ARRAY))

#define array_capacity(ARRAY)                                                  \
    (array_dtl_check(ARRAY), array_dtl_get_capacity(ARRAY))

#define array_isempty(ARRAY) (array_dtl_check(ARRAY), array_size(ARRAY) == 0)

#define array_isinline(ARRAY)                                                  \
    (array_dtl_check(ARRAY), array_dtl_is_inline(ARRAY))

#define array_reserve(ARRAY, N)                                                \
    (array_dtl_check(ARRAY),                                                   \
     array_dtl_reserve((void**)(&(ARRAY)), N, sizeof((ARRAY)[0])))

#define array_fit(ARRAY)                                                       \
    (array_dtl_check(ARRAY),                                                   \
     (((ARRAY) && (!array_dtl_is_inline(ARRAY)))                               \
        ? array_dtl_fit((void**)(&(ARRAY)), sizeof((ARRAY)[0]))                \
        : 0))

#define array_free(ARRAY)                                                      \
    (array_dtl_check(ARRAY),                                                   \
     (((ARRAY) && !array_dtl_is_inline(ARRAY))                                 \
        ? ARRAY_FREE(array_dtl_get_header(ARRAY))                              \
        : (void)0))

#define array_clear(ARRAY)                                                     \
    (array_dtl_check(ARRAY),                                                   \
     ((ARRAY) ? array_dtl_clear(array_dtl_get_header(ARRAY)) : (void)0))

#define array_push(ARRAY, VALUE)                                               \
    (array_dtl_check(ARRAY),                                                   \
     (array_dtl_need_resize((ARRAY), 1) &&                                     \
      array_dtl_reserve((void**)(&(ARRAY)),                                    \
                        array_dtl_get_size(ARRAY) + 1,                         \
                        sizeof((ARRAY)[0])))                                   \
       ? (ARRAY_ENOMEM)                                                        \
       : ((ARRAY)[array_dtl_get_size_and_incr((ARRAY), 1)] = (VALUE), 0))

#define array_pushn(ARRAY, VALUES_PTR, N)                                      \
    (array_dtl_check(ARRAY),                                                   \
     (array_dtl_need_resize((ARRAY), (N)) &&                                   \
      array_dtl_reserve((void**)(&(ARRAY)),                                    \
                        array_dtl_get_size(ARRAY) + (N),                       \
                        sizeof((ARRAY)[0])))                                   \
       ? (ARRAY_ENOMEM)                                                        \
       : (memcpy((ARRAY) + array_dtl_get_size_and_incr((ARRAY), N),            \
                 (VALUES_PTR),                                                 \
                 sizeof((ARRAY)[0]) * (N)),                                    \
          0))

#define array_pop(ARRAY)                                                       \
    (array_dtl_check(ARRAY), (ARRAY)[array_dtl_decr_and_get_size((ARRAY), 1)])

#define array_popn(ARRAY, N)                                                   \
    (array_dtl_check(ARRAY), (void)array_dtl_decr_and_get_size((ARRAY), N))

#define array_popfit(ARRAY) ((void)(array_pop(ARRAY)), array_fit(ARRAY))

#define array_popnfit(ARRAY, N) ((void)(array_popn(ARRAY, N)), array_fit(ARRAY))

#define ARRAY_INLINE_STORAGE(N, TYPE)                                          \
    (sizeof(array_header) + ((((N) + 1) & (~1)) * sizeof(TYPE)))

#define array_inline(BUFFER, N) array_dtl_inline_init((BUFFER), (N))

#endif // ARRAY_H_

//
// Implementation:
//

#ifdef ARRAY_IMPLEMENTATION

// Enforce that the capacity is at least dest_size. Will trigger realloc if
// needed. Retunrs non-zero on failure.
ARRAY_FUNC_DECORATOR int
array_dtl_reserve(void** array_ptr, ptrdiff_t dest_size, ptrdiff_t item_size)
{
    void* array = *array_ptr;
    ptrdiff_t curr_size = array_dtl_get_size(array);
    ptrdiff_t curr_capacity = array_dtl_get_capacity(array);
    if (dest_size > curr_capacity)
    {
        ptrdiff_t new_capacity = dest_size;
        if (new_capacity < 2 * curr_capacity)
            new_capacity = 2 * curr_capacity;
        if (new_capacity < ARRAY_MIN_CAPACITY)
            new_capacity = ARRAY_MIN_CAPACITY;
        new_capacity = (new_capacity + 1) & (~1);

        int was_inline = array_dtl_is_inline(array);
        ptrdiff_t alloc_total_cap = new_capacity * item_size;
        size_t alloc_size = sizeof(array_header) + (size_t)alloc_total_cap;
        void* reloc_ptr = array_dtl_get_header(array);
        if (array_dtl_is_inline(array))
            reloc_ptr = NULL;

        // realloc NULL is equivalent to malloc.
        void* new_arr = ARRAY_REALLOC(reloc_ptr, alloc_size);
        if (ARRAY_UNLIKELY(!new_arr))
            return ARRAY_ENOMEM;

        array_header* array_head = (array_header*)(new_arr);
        array_head->capacity = new_capacity;
        array_head->size = curr_size;
        *array_ptr = ((char*)new_arr) + sizeof(array_header);

        // If we were allocated on the stack, we must copy the memory.
        if (was_inline)
        {
            ptrdiff_t copy_size = item_size * curr_size;
            memcpy(*array_ptr, array, (size_t)copy_size);
        }
    }

    return 0;
}

ARRAY_FUNC_DECORATOR int
array_dtl_fit(void** array_ptr, ptrdiff_t item_size)
{
    void* array = *array_ptr;
    ARRAY_ASSERT(array != 0);
    ARRAY_ASSERT(!array_dtl_is_inline(array));

    array_header* curr_head = array_dtl_get_header(array);
    ptrdiff_t new_capacity = 2 * curr_head->size;
    if (new_capacity < ARRAY_MIN_CAPACITY)
        new_capacity = ARRAY_MIN_CAPACITY;

    if (2 * new_capacity <= curr_head->capacity)
    {
        size_t alloc_size =
          sizeof(array_header) + (size_t)(new_capacity * item_size);
        void* new_mem = ARRAY_REALLOC(array_dtl_get_header(array), alloc_size);
        if (ARRAY_UNLIKELY(!new_mem))
            return ARRAY_ENOMEM;

        array_header* array_head = (array_header*)(new_mem);
        array_head->capacity = new_capacity;
        *array_ptr = ((char*)new_mem) + sizeof(array_header);
        ARRAY_ASSERT(array_head->size <= array_head->capacity);
    }

    return 0;
}

// Inline functions gets _declared_ in the implementation.
ARRAY_FUNC_DECORATOR inline array_header*
array_dtl_get_header(void* array);

ARRAY_FUNC_DECORATOR inline ptrdiff_t
array_dtl_get_size(void* array);

ARRAY_FUNC_DECORATOR inline ptrdiff_t
array_dtl_get_capacity(void* array);

ARRAY_FUNC_DECORATOR inline int
array_dtl_need_resize(void* array, ptrdiff_t n);

ARRAY_FUNC_DECORATOR inline int
array_dtl_is_inline(void* array);

ARRAY_FUNC_DECORATOR inline void
array_dtl_check(void* array);

ARRAY_FUNC_DECORATOR inline ptrdiff_t
array_dtl_get_size_and_incr(void* array, ptrdiff_t n);

ARRAY_FUNC_DECORATOR inline ptrdiff_t
array_dtl_decr_and_get_size(void* array, ptrdiff_t n);

ARRAY_FUNC_DECORATOR inline void*
array_dtl_inline_init(void* mem, ptrdiff_t capacity);

#endif // ARRAY_IMPLEMENTATION
