#ifndef ARENA_H_
#define ARENA_H_

#define ARENA_NODE_SIZE (1 << 16)
#define ARENA_MAX_ALLOC_SIZE (((mm)(ARENA_NODE_SIZE - sizeof(arena_node*) - sizeof(mm))))

typedef struct arena_node arena_node;
struct arena_node
{
    mm idx;
    arena_node* next;
    char mem[ARENA_NODE_SIZE - sizeof(arena_node*) - sizeof(mm)];
};

typedef struct arena arena;
struct arena
{
    arena_node* head;
};

static char* arena_allocate(arena* a, mm size);

#endif // ARENA_H_

#ifdef ARENA_IMPLEMENTATION

static char*
arena_allocate(arena* a, mm size)
{
    assert(size < ARENA_MAX_ALLOC_SIZE);
    assert(a);

    if (LIKELY(a->head != 0 && a->head->idx + size <= ARENA_MAX_ALLOC_SIZE)) // TODO: Make sure its "<="
    arena_bump_ptr:
    {
        char* retval = a->head->mem + a->head->idx;
        a->head->idx += ((size + 15) & (~0xF)); // Round to alignment

        return retval;
    }

    arena_node* node = malloc(sizeof(arena_node));
    arena_node* prev_head = a->head;
    a->head = node;
    node->next = prev_head;
    node->idx = 0;

    goto arena_bump_ptr;
}

#endif // ARENA_IMPLEMENTATION
