// Defines:
//
//   * #define HASHMAP_IMPLEMENTATION
//         For this file to behave like an implementation file. Without this it
//         behaves like a header file. Note that this file mostly contains macros
//         so the only thing to instantiate is a hash function.
//
//   * #define HASHMAP_STATIC
//         To make an implementation private to the translation unit.
//
//   * #define HASHMAP_MIN_CAPACITY
//         To set minimum number of items in one allocation. Array capacity, if
//         not zero, will never be smaller than this value. If not provided,
//         sensible default one is used. Provided value MUST BE DIVISIBLE by 2.
//         TODO: Or is it? Shoudn't it be a power of 2?
//         TODO: Rename, so that it's clear that its num of chars to insert not
//               to hold.
//
//   * #define HASHMAP_DEFAULT_HASH
//         To replace a Murmur3 default hash function. Note that if you deine
//         this one, Murmur code is never instantiated so HASHMAP_IMPLEMENATION
//         macros becomes usless. Hash function must be invokable with a ptr to
//         item to hash and number of bytes.
//
//   * #define HASHMAP_NOERRNO
//         If you don't want to include errno.h. Some functions return errno
//         values that reflects errors (errno is NEVER set). If this is defined
//         they will only return 0/1.
//
//   * #define HASHMAP_ASSERT
//         If not defined stdlib assert header is included and a default assert
//         macro is used.
//
//   * #define HASHMAP_STRLEN
//   * #define HASHMAP_STRCMP
//   * #define HASHMAP_MEMCPY
//         To replace one or more of these functions. Note that if any of these
//         is defined, string.h is included and missing functions are added.
//
//   * #define HASHMAP_MALLOC
//   * #define HASHMAP_FREE
//         To replace malloc and free. You must define either both of them or
//         none.
//
// Usage:
//
//   * You must first INSTANTIATE the hashmap yourself with given key/value
//     types, compare, hash funcs etc. Defining a hasmap looks like this:
//
//     // In the header file:
//     HASHMAP_DECLARE(map_name, key_t, value_t);
//     #define map_name_insert(HM, K, V) map_name_insert_((void**)&(HM), K, V)
//     #define map_name_reserve(HM, N) map_name_reserve_((void**)&(HM), N)
//
//     // In the implementation file:
//     HASHMAP_DEFINE(map_name, key_t, value_t,
//                    HASHMAP_DEFAULT_COMPARE,
//                    HASHMAP_DEFAULT_HASH);
//
//     If you defined HASHMAP_STATIC with the intention of pulling the header
//     once into the file you can of course DEFINE map right after DECLAREing
//     it. The macros are necesarry, as C doesn't allow you to define macro in a
//     macro. Instead of HASHMAP_DEFAULT_COMPARE you can provide a function
//     which is invokable like: int(*)(void const*, void const*) , where pointer
//     params are pointers to the things you want to compare (like qsort param).
//     Instead of HASHMAP_DEFAULT_HASH you can provide a hash function invokable
//     with pointer to the key and size of the key with signature:
//     (size_t)(void const* key, ptrdiff_t len) .
//
//    * TODO: DESCRIBE THE MAP INTERFACE!
//
// Notes:
//
//   * If you want to use the "conforming" murmur3 or my murmur3 w/ small cases
//     fallbacks implemented here for general purpose hashing you can use it,
//     just #define 'your_hash_func_name' to either 'hashmap_murmur3' or
//     'hashmap_hashbytes'. This is me, trying not to invade your namespace.
//
// TODO:
//
//   * Combine_hash are the same for 32 and 64-bits. This might be wrong.
//
//   * Add inline (onstack) functionality.

#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <stddef.h>

#ifdef HASHMAP_STATIC
#  define HASHMAP_FUNC_DECORATOR static
#else
#  ifdef __cplusplus
#    define HASHMAP_FUNC_DECORATOR extern "C"
#  else
#    define HASHMAP_FUNC_DECORATOR extern
#  endif
#endif

#ifndef HASHMAP_ASSERT
#  include <assert.h>
#  define HASHMAP_ASSERT assert
#endif // HASHMAP_ASSERT

#ifndef HASHMAP_NOERRNO
#  include <errno.h>
#  define HASHMAP_ENOMEM ENOMEM
#else
#  define HASHMAP_ENOMEM 1 // No errno so any non zero value means error.
#endif // HASHMAP_NOERRNO

#if !defined(HASHMAP_MALLOC) && !defined(HASHMAP_FREE)
#  include <stdlib.h>
#  define HASHMAP_MALLOC malloc
#  define HASHMAP_FREE free
#else
#  if !defined(HASHMAP_MALLOC) || !defined(HASHMAP_FREE)
#    error You must define but malloc and free or none of them.
#  endif
#endif // !defined(HASHMAP_MALLOC) && !defined(HASHMAP_FREE)

#ifndef HASHMAP_MEMCPY
#  include <string.h>
#  define HASHMAP_MEMCPY memcpy
#endif // HASHMAP_MEMCPY

#ifndef HASHMAP_STRCMP
#  include <string.h>
#  define HASHMAP_STRCMP strcmp
#endif // HASHMAP_STRCMP

#ifndef HASHMAP_STRLEN
#  include <string.h>
#  define HASHMAP_STRLEN strlen
#endif // HASHMAP_STRLEN

#ifndef HASHMAP_MIN_CAPACITY
#  define HASHMAP_MIN_CAPACITY (32L)
#endif // HASHMAP_MIN_CAPACITY

#if (defined(__GNUC__) || defined(__clang__))
#  define HASHMAP_LIKELY(EXPR) __builtin_expect((EXPR), 1)
#  define HASHMAP_UNLIKELY(EXPR) __builtin_expect((EXPR), 0)
#else
#  define HASHMAP_LIKELY(EXPR) (EXPR)
#  define HASHMAP_UNLIKELY(EXPR) (EXPR)
#endif

// Invokable with two pointers to values that should be compare. The semantics
// are same as func of type: int(*)(const void*, const void*) , and is
// intended to be a drop-in replacement, when values can be compared with == .
#define HASHMAP_DEFAULT_COMPARE(X, Y) (!((*(X)) == (*(Y))))

#ifndef HASHMAP_DEFAULT_HASH
#  define HASHMAP_DEFAULT_HASH(X, N) hashmap_hashbytes(X, N)
#else
#  define HASHMAP_DEFAULT_HASH_FUNCTION_PROVIDED
#endif // HASHMAP_DEFAULT_HASH

// Compare strings with strcmp. Semantics same as in function declared as:
// int(*)(const void*, const void*)
#define HASHMAP_STRINGZ_COMPARE(X, Y)                                          \
    HASHMAP_STRCMP((char const*)(*(X)), (char const*)(*(Y)))

// Compare strings using default hash. N is discared, and first the length is
// computed with strlen, so if you really, really care about speed of this part,
// you should replace it with a func that hases zero-terminated stream.
#define HASHMAP_STRINGZ_HASH(X, N)                                             \
    HASHMAP_DEFAULT_HASH(                                                      \
        *(char const* const*)(void const*)(X),                                 \
        (ptrdiff_t)HASHMAP_STRLEN(*(char const* const*)(void const*)(X)))

// New clang does not like /* FALL THROUGH */ comment annotations, so in order
// to silience some stupid warnings, insert this attribute instead. Note that we
// also have to add a comment annotation for other compilers...
#if defined(__clang__) && defined(__clang_major__) && (__clang_major__ >= 10)
#  define HASHMAP_FALLTHROUGH __attribute__((fallthrough));
#else
#  define HASHMAP_FALLTHROUGH
#endif

// Try to guess the ptrsize and assert it's correct.
#if !defined(HASHMAP_PTRSIZE_32) && !defined(HASHMAP_PTRSIZE_64)
#  if defined(_M_X64) || defined(_M_AMD64) || defined(_WIN64)                  \
      || defined(__amd64__) || defined(__amd64)                                \
      || defined(__x86_64__) || defined(__x86_64)
#    define HASHMAP_PTRSIZE_64
typedef char hashmap_size_t_size[(sizeof(size_t) == 8) ? 1 : -1];
#  elif defined(_M_IX86)                                                       \
        || defined(i386) || defined(__i386) || defined(__i386__)
#    define HASHMAP_PTRSIZE_32
typedef char hashmap_size_t_size[(sizeof(size_t) == 4) ? 1 : -1];
#  else
#    error Cannot determine an arch to use! #define HASHMAP_PTRSIZE_32/64
#  endif
#endif

// Reasonable way to force semicolon, which produces understandable warnings
// across most compilers.
#define HASHMAP_CONCAT_IMPL(X, Y) X##Y
#define HASHMAP_CONCAT(X, Y) HASHMAP_CONCAT_IMPL(X, Y)
#define HASHMAP_FORCE_SEMICOLON()                                              \
    struct HASHMAP_CONCAT(md_hmap_impl_unused_struct_ , __LINE__)              \
    {                                                                          \
        char ignr_pad_;                                                        \
    }

// TODO: Check if windows requires byteswap here.
#  define HASHMAP_LOADU_WORD(DESTPTR, SRCPTR)                                  \
    HASHMAP_MEMCPY((DESTPTR), (SRCPTR), sizeof(size_t))

// Good old Bit Twiddling Hacks
#if defined(HASHMAP_PTRSIZE_64)

#  define HASHMAP_SEED_1 0x319642b2d24d8ec3ULL
#  define HASHMAP_SEED_2 0x96de1b173f119089ULL

#  ifdef _MSC_VER
#    define HASHMAP_ROTATE_WORD_L(X, R) (_rotl64((X), (R)))
#  else
#    define HASHMAP_ROTATE_WORD_L(X, R) (((X) << (R)) | ((X) >> (64 - (R))))
#  endif

#  define HASHMAP_MURMUR_FMIX(X)                                               \
    do {                                                                       \
        (X) ^= (X) >> 33;                                                      \
        (X) *= 0xff51afd7ed558ccdULL;                                          \
        (X) ^= (X) >> 33;                                                      \
        (X) *= 0xc4ceb9fe1a85ec53ULL;                                          \
        (X) ^= (X) >> 33;                                                      \
    } while (0)

#  define HASHMAP_ROUND_WORD_2POWER(X)                                         \
    do {                                                                       \
        (X)--;                                                                 \
        (X) |= (X) >> 1;                                                       \
        (X) |= (X) >> 2;                                                       \
        (X) |= (X) >> 4;                                                       \
        (X) |= (X) >> 8;                                                       \
        (X) |= (X) >> 16;                                                      \
        (X) |= (X) >> 32;                                                      \
        (X)++;                                                                 \
    } while (0)
#elif defined(HASHMAP_PTRSIZE_32)

#  define HASHMAP_SEED_1 0xd24d8ec3
#  define HASHMAP_SEED_2 0x3f119089

#  ifdef _MSC_VER
#    define HASHMAP_ROTATE_WORD_L(X, R) (_rotl((X), (R)))
#  else
#    define HASHMAP_ROTATE_WORD_L(X, R) (((X) << (R)) | ((X) >> (32 - (R))))
#  endif

#  define HASHMAP_MURMUR_FMIX(X)                                               \
    do {                                                                       \
        (X) ^= (X) >> 16;                                                      \
        (X) *= 0x85ebca6b;                                                     \
        (X) ^= (X) >> 13;                                                      \
        (X) *= 0xc2b2ae35;                                                     \
        (X) ^= (X) >> 16;                                                      \
    } while (0)

#  define HASHMAP_ROUND_WORD_2POWER(X)                                         \
    do {                                                                       \
        (X)--;                                                                 \
        (X) |= (X) >> 1;                                                       \
        (X) |= (X) >> 2;                                                       \
        (X) |= (X) >> 4;                                                       \
        (X) |= (X) >> 8;                                                       \
        (X) |= (X) >> 16;                                                      \
        (X)++;                                                                 \
    } while (0)
#endif

// This trick is taken from boost::hash combine. The idea is to xor hashes but
// wiggle then a little bit in case they are bad or something. The number
// constant seems to be a golden ratio inverse.
#define HASHMAP_COMBINE_HASHES(H1, H2)                                         \
    ((H1) ^ ((H2) + ((size_t)0x9e3779b9) + ((H1) << 6) + ((H1) >> 2)))

typedef struct hashmap_header
{
    ptrdiff_t load; // Avoid name 'size' because it would be misleading.
    ptrdiff_t capacity;
} hashmap_header;

#define HASHMAP_DECLARE(NAME, KEY_TYPE, VALUE_TYPE)                            \
    HASHMAP_DECLARE_IMPL(HASHMAP_FUNC_DECORATOR, NAME, KEY_TYPE, VALUE_TYPE)

#define HASHMAP_DEFINE(NAME, KEY_TYPE, VALUE_TYPE,                             \
                       COMPARE_FUNC, HASH_FUNC)                                \
    HASHMAP_DEFINE_IMPL(HASHMAP_FUNC_DECORATOR, NAME, KEY_TYPE, VALUE_TYPE,    \
                        COMPARE_FUNC, HASH_FUNC)

#define HASHMAP_DECLARE_IMPL(DECORATOR, NAME, KEY_TYPE, VALUE_TYPE)            \
    struct NAME##_kvp                                                          \
    {                                                                          \
        int away;                                                              \
        KEY_TYPE key;                                                          \
        VALUE_TYPE value;                                                      \
    };                                                                         \
    typedef struct NAME##_kvp NAME##_kvp;                                      \
                                                                               \
    DECORATOR int NAME##_contains_item(NAME##_kvp const p);                    \
                                                                               \
    DECORATOR ptrdiff_t NAME##_load(void* hmap);                               \
                                                                               \
    DECORATOR ptrdiff_t NAME##_capacity(void* hmap);                           \
                                                                               \
    DECORATOR void NAME##_free(void* hmap);                                    \
                                                                               \
    DECORATOR void NAME##_dtl_insert_unsafe(NAME##_kvp* const hmap,            \
                                         KEY_TYPE const key,                   \
                                         VALUE_TYPE const value);              \
                                                                               \
    DECORATOR int NAME##_reserve_(void** hmap_ptr, ptrdiff_t nmemb);           \
                                                                               \
    DECORATOR ptrdiff_t NAME##_insert_(void** hmap_ptr,                        \
                                    KEY_TYPE const key,                        \
                                    VALUE_TYPE const value);                   \
                                                                               \
    DECORATOR ptrdiff_t NAME##_findi(NAME##_kvp* hmap,                         \
                                  KEY_TYPE const key);                         \
                                                                               \
    DECORATOR VALUE_TYPE* NAME##_findp(NAME##_kvp* hmap,                       \
                                    KEY_TYPE const key);                       \
                                                                               \
    DECORATOR int NAME##_exists(NAME##_kvp* hmap, KEY_TYPE const key);         \
                                                                               \
    DECORATOR int NAME##_delete(NAME##_kvp* hmap, KEY_TYPE const key);         \
                                                                               \
    DECORATOR int NAME##_deleteat(NAME##_kvp* hmap, ptrdiff_t idx);            \
                                                                               \
    HASHMAP_FORCE_SEMICOLON()

#define HASHMAP_DEFINE_IMPL(DECORATOR, NAME, KEY_TYPE, VALUE_TYPE,             \
                            COMPARE_FUNC, HASH_FUNC)                           \
    DECORATOR inline int                                                       \
    NAME##_has_item(NAME##_kvp* hmap, ptrdiff_t idx)                           \
    {                                                                          \
        return hmap[idx].away != -1;                                           \
    }                                                                          \
                                                                               \
    DECORATOR inline ptrdiff_t                                                 \
    NAME##_load(void* hmap)                                                    \
    {                                                                          \
        if (HASHMAP_UNLIKELY(hmap == NULL))                                    \
            return 0;                                                          \
                                                                               \
        hashmap_header* head = (hashmap_header*)(void*)(                       \
            ((char*)hmap) - sizeof(hashmap_header));                           \
        return head->load;                                                     \
    }                                                                          \
                                                                               \
    DECORATOR inline ptrdiff_t                                                 \
    NAME##_capacity(void* hmap)                                                \
    {                                                                          \
        if (HASHMAP_UNLIKELY(hmap == NULL))                                    \
            return 0;                                                          \
                                                                               \
        hashmap_header* head = (hashmap_header*)(void*)(                       \
            ((char*)hmap) - sizeof(hashmap_header));                           \
        return head->capacity;                                                 \
    }                                                                          \
                                                                               \
    DECORATOR inline void                                                      \
    NAME##_free(void* hmap)                                                    \
    {                                                                          \
        if (hmap)                                                              \
        {                                                                      \
            void* head = (void*)(((char*)hmap) - sizeof(hashmap_header));      \
            free(head);                                                        \
        }                                                                      \
    }                                                                          \
                                                                               \
    DECORATOR void                                                             \
    NAME##_dtl_insert_unsafe(NAME##_kvp* const hmap,                           \
                             KEY_TYPE const key,                               \
                             VALUE_TYPE const value)                           \
    {                                                                          \
        hashmap_header* head = (hashmap_header*)(void*)(                       \
            ((char*)hmap) - sizeof(hashmap_header));                           \
        HASHMAP_ASSERT(head->load < head->capacity);                           \
                                                                               \
        NAME##_kvp inserted;                                                   \
        inserted.away = 0;                                                     \
        inserted.key = key;                                                    \
        inserted.value = value;                                                \
                                                                               \
        size_t hash = (size_t)HASH_FUNC((void const*)(&key), sizeof(KEY_TYPE));\
        ptrdiff_t idx = (ptrdiff_t)(hash & (size_t)(head->capacity - 1));      \
        for (;;)                                                               \
        {                                                                      \
            if (hmap[idx].away == -1)                                          \
            {                                                                  \
                hmap[idx] = inserted;                                          \
                head->load++;                                                  \
                break;                                                         \
            }                                                                  \
                                                                               \
            if (!(COMPARE_FUNC(&(hmap[idx].key), &(inserted.key))))            \
            {                                                                  \
                NAME##_kvp temp = hmap[idx];                                   \
                hmap[idx] = inserted;                                          \
                                                                               \
                /* TODO: Handle old element destruction (now in temp) */       \
                (void)temp;                                                    \
                break;                                                         \
            }                                                                  \
                                                                               \
            if (hmap[idx].away < inserted.away)                                \
            {                                                                  \
                NAME##_kvp temp = inserted;                                    \
                inserted = hmap[idx];                                          \
                hmap[idx] = temp;                                              \
            }                                                                  \
                                                                               \
            idx = (idx + 1) & (head->capacity - 1);                            \
            inserted.away++;                                                   \
            /* If away rotates to 0, then it probably means there is a */      \
            /* a terrrible collision out there. Like 2kkk elements! It's */    \
            /* probably not worh to check for in. */                           \
        }                                                                      \
    }                                                                          \
                                                                               \
    /* nmemb is how many items once can insert before growing the table. So */ \
    /* given the load factor is 0.5, it means calling reserve with eg 29 it */ \
    /* will allocate space for 64 nodes without any grow. */                   \
    DECORATOR int                                                              \
    NAME##_reserve_(void** hmap_ptr, ptrdiff_t nmemb)                          \
    {                                                                          \
        void* mem = *hmap_ptr;                                                 \
        NAME##_kvp* hmap = (NAME##_kvp*)mem;                                   \
        hashmap_header* head = hmap                                            \
            ? (hashmap_header*)(void*)(((char*)hmap) - sizeof(hashmap_header)) \
            : NULL;                                                            \
        ptrdiff_t prev_capacity = head ? head->capacity: 0;                    \
        int needs_free = head ? 1 : 0; /* TODO: if head, check if is_inline */ \
                                                                               \
        nmemb *= 2;                                                            \
        if (nmemb < HASHMAP_MIN_CAPACITY)                                      \
            nmemb = HASHMAP_MIN_CAPACITY;                                      \
                                                                               \
        /* This is actually likely to happen, because we check some */         \
        /* conditions before jumping into this function, so usunally when we*/ \
        /* are in reserve, we need to reserve. */                              \
        if (HASHMAP_LIKELY(nmemb > prev_capacity))                             \
        {                                                                      \
            ptrdiff_t new_capacity = nmemb;                                    \
            HASHMAP_ASSERT(new_capacity > 0);                                  \
            HASHMAP_ROUND_WORD_2POWER(new_capacity);                           \
            HASHMAP_ASSERT(new_capacity >= 2 * prev_capacity);                 \
                                                                               \
            void* new_mem = malloc(                                            \
                sizeof(hashmap_header)                                         \
                + (size_t)new_capacity * sizeof(NAME##_kvp));                  \
                                                                               \
            if (!new_mem)                                                      \
                return ENOMEM;                                                 \
                                                                               \
            hashmap_header* new_head = (hashmap_header*)new_mem;               \
            new_head->load = 0;                                                \
            new_head->capacity = new_capacity;                                 \
                                                                               \
            NAME##_kvp* new_map = (NAME##_kvp*)(void*)(                        \
                (char*)new_mem + sizeof(hashmap_header));                      \
            for (ptrdiff_t i = 0; i < new_capacity; ++i)                       \
                new_map[i].away = -1;                                          \
                                                                               \
            /* This actually is slow. I have in mind an idea for faster */     \
            /* grow algorithm, that would even be able to take advantage */    \
            /* of realloc, but it's pretty bugprone, so maybe someday. */      \
            for (ptrdiff_t i = 0; i < prev_capacity; ++i)                      \
            {                                                                  \
                if (hmap[i].away == -1)                                        \
                    continue;                                                  \
                                                                               \
                NAME##_dtl_insert_unsafe(new_map, hmap[i].key, hmap[i].value); \
            }                                                                  \
                                                                               \
            if (HASHMAP_LIKELY(needs_free))                                    \
            {                                                                  \
                char* mem_root = ((char*)hmap) - sizeof(hashmap_header);       \
                free(mem_root);                                                \
            }                                                                  \
                                                                               \
            *hmap_ptr = new_map;                                               \
        }                                                                      \
                                                                               \
        return 0;                                                              \
    }                                                                          \
                                                                               \
    DECORATOR ptrdiff_t                                                        \
    NAME##_insert_(void** hmap_ptr, KEY_TYPE const key, VALUE_TYPE const value)\
    {                                                                          \
        HASHMAP_ASSERT(hmap_ptr != NULL);                                      \
        char* hmap = (char*)(*hmap_ptr);                                       \
        ptrdiff_t prev_load = 0;                                               \
        ptrdiff_t prev_capacity = 0;                                           \
        hashmap_header* head = NULL;                                           \
                                                                               \
        if (HASHMAP_UNLIKELY(hmap == NULL))                                    \
            goto realloc_anyway;                                               \
                                                                               \
        head = (hashmap_header*)(void*)(hmap - sizeof(hashmap_header));        \
        prev_load = head->load;                                                \
        prev_capacity = head->capacity;                                        \
                                                                               \
        if (HASHMAP_UNLIKELY(2 * (prev_load + 1) > prev_capacity))             \
        realloc_anyway:                                                        \
        {                                                                      \
            int grow_failed = NAME##_reserve_(hmap_ptr, 2 * prev_load);        \
            if (HASHMAP_UNLIKELY(grow_failed))                                 \
                return grow_failed;                                            \
                                                                               \
            hmap = (char*)(*hmap_ptr);                                         \
        }                                                                      \
                                                                               \
        HASHMAP_ASSERT(hmap);                                                  \
        NAME##_dtl_insert_unsafe((NAME##_kvp *)(void*)hmap, key, value);       \
        return 0;                                                              \
    }                                                                          \
                                                                               \
    DECORATOR ptrdiff_t                                                        \
    NAME##_findi(NAME##_kvp* hmap, KEY_TYPE const key)                         \
    {                                                                          \
        if (HASHMAP_UNLIKELY(hmap == NULL))                                    \
            return -1;                                                         \
                                                                               \
        hashmap_header* head = (hashmap_header*)(void*)(                       \
            ((char*)hmap) - sizeof(hashmap_header));                           \
                                                                               \
        size_t hash = (size_t)HASH_FUNC((char const*)(&key), sizeof(KEY_TYPE));\
        ptrdiff_t idx = (ptrdiff_t)(hash & (size_t)(head->capacity - 1));      \
        for (;;)                                                               \
        {                                                                      \
            if (hmap[idx].away == -1)                                          \
                return -1;                                                     \
                                                                               \
            if (!COMPARE_FUNC(&(hmap[idx].key), &(key)))                       \
                return idx;                                                    \
                                                                               \
            idx = (idx + 1) & (head->capacity - 1);                            \
        }                                                                      \
    }                                                                          \
                                                                               \
    DECORATOR VALUE_TYPE*                                                      \
    NAME##_findp(NAME##_kvp* hmap, KEY_TYPE const key)                         \
    {                                                                          \
        ptrdiff_t idx = NAME##_findi(hmap, key);                               \
        return idx >= 0 ? &(hmap[idx].value) : NULL;                           \
    }                                                                          \
                                                                               \
    DECORATOR int                                                              \
    NAME##_exists(NAME##_kvp* hmap, KEY_TYPE const key)                        \
    {                                                                          \
        ptrdiff_t idx = NAME##_findi(hmap, key);                               \
        return idx >= 0;                                                       \
    }                                                                          \
                                                                               \
    /* TODO: Merge delete and deleteat */                                      \
    DECORATOR int                                                              \
    NAME##_delete(NAME##_kvp* hmap, KEY_TYPE const key)                        \
    {                                                                          \
        if (HASHMAP_UNLIKELY(hmap == NULL))                                    \
            return 0;                                                          \
                                                                               \
        hashmap_header* head = (hashmap_header*)(void*)(                       \
            ((char*)hmap) - sizeof(hashmap_header));                           \
        size_t hash = (size_t)HASH_FUNC((char const*)(&key), sizeof(KEY_TYPE));\
        ptrdiff_t idx = (ptrdiff_t)(hash & (size_t)(head->capacity - 1));      \
        ptrdiff_t found_idx = -1;                                              \
        for (;;)                                                               \
        {                                                                      \
            /* No element to delete. */                                        \
            if (hmap[idx].away == -1)                                          \
                return 0;                                                      \
                                                                               \
            if (!COMPARE_FUNC(&(hmap[idx].key), &(key)))                       \
            {                                                                  \
                found_idx = idx;                                               \
                break;                                                         \
            }                                                                  \
                                                                               \
            idx = (idx + 1) & (head->capacity - 1);                            \
        }                                                                      \
                                                                               \
        HASHMAP_ASSERT(found_idx >= 0);                                        \
        ptrdiff_t next_idx = (found_idx + 1) & (head->capacity - 1);           \
                                                                               \
        while (hmap[next_idx].away > 0)                                        \
        {                                                                      \
            hmap[next_idx].away--;                                             \
            NAME##_kvp temp = hmap[found_idx];                                 \
            hmap[found_idx] = hmap[next_idx];                                  \
            hmap[next_idx] = temp;                                             \
            found_idx = (found_idx + 1) & (head->capacity - 1);                \
            next_idx = (next_idx + 1) & (head->capacity - 1);                  \
        }                                                                      \
        hmap[found_idx].away = -1;                                             \
        head->load--;                                                          \
        HASHMAP_ASSERT(head->load >= 0);                                       \
                                                                               \
        return 1;                                                              \
    }                                                                          \
                                                                               \
    DECORATOR int                                                              \
    NAME##_deleteat(NAME##_kvp* hmap, ptrdiff_t idx)                           \
    {                                                                          \
        HASHMAP_ASSERT(hmap != NULL);                                          \
        HASHMAP_ASSERT(idx >= 0);                                              \
        HASHMAP_ASSERT(hmap[idx].away >= 0);                                   \
                                                                               \
        hashmap_header* head = (hashmap_header*)(void*)(                       \
            ((char*)hmap) - sizeof(hashmap_header));                           \
        ptrdiff_t next_idx = (idx + 1) & (head->capacity - 1);                 \
                                                                               \
        while (hmap[next_idx].away > 0)                                        \
        {                                                                      \
            hmap[next_idx].away--;                                             \
            NAME##_kvp temp = hmap[idx];                                       \
            hmap[idx] = hmap[next_idx];                                        \
            hmap[next_idx] = temp;                                             \
            idx = (idx + 1) & (head->capacity - 1);                            \
            next_idx = (next_idx + 1) & (head->capacity - 1);                  \
        }                                                                      \
        hmap[idx].away = -1;                                                   \
        head->load--;                                                          \
        HASHMAP_ASSERT(head->load >= 0);                                       \
                                                                               \
        return 1;                                                              \
    }                                                                          \
                                                                               \
    HASHMAP_FORCE_SEMICOLON()

#ifndef HASHMAP_DEFAULT_HASH_FUNCTION_PROVIDED

HASHMAP_FUNC_DECORATOR size_t hashmap_murmur3(void const* key, ptrdiff_t len);

// In key is small (small here means up to two size_ts'), jump straight to the
// finalization part. It's probably not as high quality as sending it through
// the regular pipeline, but is way faster and since hashmap keys are usually
// small (expect strings of course), it's worth to optimize, even when giving up
// the hash quality. Testing with SMHasher (also see murmur github) reveals some
// horrible problems with the "Keyset 'TwoBytes' Tests", but rest of the tests
// (including small ones) are fine. Also small cases with len known at compile
// time (like sizeof(int)) will (or at least should) be inlined by the compier.
HASHMAP_FUNC_DECORATOR inline size_t
hashmap_hashbytes(void const* key, ptrdiff_t const len)
{
    if (HASHMAP_LIKELY((size_t)len > 2 * sizeof(size_t)))
    {
        return hashmap_murmur3(key, len);
    }
    else
    {
        union
        {
            size_t x[2];
            char bytes[2 * sizeof(size_t)];
        } u;
        u.x[0] = HASHMAP_SEED_1; // Hardcoded seed
        u.x[1] = HASHMAP_SEED_2; // Hardcoded seed

        // memcpy seems to be the fastest way to get these bytes but I still
        // want to replace it.
        HASHMAP_MEMCPY(u.bytes, key, (size_t)len);

        if ((size_t)len <= sizeof(size_t))
        {
            HASHMAP_MURMUR_FMIX(u.x[0]);
            return u.x[0];
        }
        else // if (len <= 2 * sizeof(size_t))
        {
            u.x[0] ^= (size_t)len;
            u.x[1] ^= (size_t)len;
            u.x[0] += u.x[1];
            u.x[1] += u.x[0];
            HASHMAP_MURMUR_FMIX(u.x[0]);
            HASHMAP_MURMUR_FMIX(u.x[1]);
            u.x[0] += u.x[1];
            u.x[1] += u.x[0];

            size_t result = HASHMAP_COMBINE_HASHES(u.x[0], u.x[1]);
            return result;
        }
    }
}
#endif // HASHMAP_DEFAULT_HASH_FUNCTION_PROVIDED

#endif // HASHMAP_H_

//
// Implementation:
//

#ifdef HASHMAP_IMPLEMENTATION

#ifndef HASHMAP_DEFAULT_HASH_FUNCTION_PROVIDED

// This is MurmurHash3 created and released into the public domain by Austin
// Appleby. For details visit: https://github.com/aappleby/smhasher Note that
// this will include either 32bit or 64bit version depending on your size_t.
// These DO NOT produce same hashes.
#if defined(HASHMAP_PTRSIZE_64)

// For 64-bit, we hash the input with a hardcoded seed and truncate the result
// into 64-bit (becasue the original is either 32 or 128).
HASHMAP_FUNC_DECORATOR size_t
hashmap_murmur3(void const* key, ptrdiff_t len)
{
    ptrdiff_t nblocks = len / 16;
    char const* data = (char const*)key;
    char const* tail = (char const*)(data + nblocks * 16);
    size_t h1 = HASHMAP_SEED_1; // Hardcoded seed
    size_t h2 = HASHMAP_SEED_2; // Hardcoded seed
    size_t c1 = 0x87c37b91114253d5ULL;
    size_t c2 = 0x4cf5ad432745937fULL;

    // body
    for(size_t const* block = (size_t const*)(void const*)data;
        block != (size_t const*)(void const*)tail;
        block += 2)
    {
        size_t k1;
        size_t k2;
        HASHMAP_LOADU_WORD(&k1, block);
        HASHMAP_LOADU_WORD(&k2, block + 1);

        k1 *= c1;
        k1 = HASHMAP_ROTATE_WORD_L(k1, 31);
        k1 *= c2;
        h1 ^= k1;
        h1 = HASHMAP_ROTATE_WORD_L(h1, 27);
        h1 += h2;
        h1 = h1 * 5 + 0x52dce729;

        k2 *= c2;
        k2 = HASHMAP_ROTATE_WORD_L(k2, 33);
        k2 *= c1;
        h2 ^= k2;
        h2 = HASHMAP_ROTATE_WORD_L(h2, 31);
        h2 += h1;
        h2 = h2 * 5 + 0x38495ab5;
    }

    // tail
    size_t k1 = 0;
    size_t k2 = 0;
    switch(len & 15) {
    case 15:
        k2 ^= ((size_t)tail[14]) << 48;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 14:
        k2 ^= ((size_t)tail[13]) << 40;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 13:
        k2 ^= ((size_t)tail[12]) << 32;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 12:
        k2 ^= ((size_t)tail[11]) << 24;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 11:
        k2 ^= ((size_t)tail[10]) << 16;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 10:
        k2 ^= ((size_t)tail[9]) << 8;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 9:
        k2 ^= ((size_t)tail[8]) << 0;
        k2 *= c2;
        k2  = HASHMAP_ROTATE_WORD_L(k2, 33);
        k2 *= c1;
        h2 ^= k2;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 8:
        k1 ^= ((size_t)tail[7]) << 56;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 7:
        k1 ^= ((size_t)tail[6]) << 48;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 6:
        k1 ^= ((size_t)tail[5]) << 40;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 5:
        k1 ^= ((size_t)tail[4]) << 32;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 4:
        k1 ^= ((size_t)tail[3]) << 24;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 3:
        k1 ^= ((size_t)tail[2]) << 16;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 2:
        k1 ^= ((size_t)tail[1]) << 8;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 1:
        k1 ^= ((size_t)tail[0]) << 0;
        k1 *= c1;
        k1  = HASHMAP_ROTATE_WORD_L(k1, 31);
        k1 *= c2;
        h1 ^= k1;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    default:
        break;
    }

    // finalization
    h1 ^= (size_t)len;
    h2 ^= (size_t)len;
    h1 += h2;
    h2 += h1;
    HASHMAP_MURMUR_FMIX(h1);
    HASHMAP_MURMUR_FMIX(h2);
    h1 += h2;
    h2 += h1;

    size_t result = HASHMAP_COMBINE_HASHES(h1, h2);
    return result;
}

#elif defined(HASHMAP_PTRSIZE_32)

HASHMAP_FUNC_DECORATOR size_t
hashmap_murmur3(void const* key, ptrdiff_t len)
{
    ptrdiff_t const nblocks = len / 4;
    char const* data = (char const*)key;
    char const* tail = (char const*)(data + nblocks * 4);
    size_t h1 = 0xd24d8ec3;
    size_t const c1 = 0xcc9e2d51;
    size_t const c2 = 0x1b873593;

    // body
    for(size_t const* block = (size_t const*)(void const*)data;
        block != (size_t const*)(void const*)tail;
        block++)
    {
        size_t k1;
        HASHMAP_LOADU_WORD(&k1, block);

        k1 *= c1;
        k1 = HASHMAP_ROTATE_WORD_L(k1, 15);
        k1 *= c2;

        h1 ^= k1;
        h1 = HASHMAP_ROTATE_WORD_L(k1, 13);
        h1 = h1 * 5 + 0xe6546b64;
    }

    // tail
    size_t k1 = 0; // Hardcoded seed
    switch(len & 3) {
    case 3:
        k1 ^= ((size_t)tail[2]) << 16;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 2:
        k1 ^= ((size_t)tail[1]) << 8;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    case 1:
        k1 ^= ((size_t)tail[0]);
        k1 *= c1;
        k1 = HASHMAP_ROTATE_WORD_L(k1, 15);
        k1 *= c2;
        h1 ^= k1;
        HASHMAP_FALLTHROUGH /* FALL THROUGH */
    default:
        break;
    }

    // finalization
    h1 ^= (size_t)len;
    HASHMAP_MURMUR_FMIX(h1);
    return h1;
}
#endif

// Inline functions gets _declared_ in the implementation.
HASHMAP_FUNC_DECORATOR inline size_t hashmap_hashbytes(void const* key, ptrdiff_t const len);

#endif // HASHMAP_DEFAULT_HASH_FUNCTION_PROVIDED
#endif // HASHMAP_IMPLEMENTATION
