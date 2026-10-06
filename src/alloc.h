#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <stddef.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

typedef struct Arena_* Arena;

#define DEFAULT_ARENA_CAPACITY 1024

struct Arena_ {
    unsigned char* buffer;
    size_t bufferCapacity;
    size_t allocatedBytes;
    Arena next;
};

void* checked_malloc(size_t bytes);

Arena Arena_makeArena(size_t capacity, Arena next);
void Arena_freeArena(Arena arenaHead);
void* Arena_allocArena(Arena* arenaHead, size_t dataSize, size_t alignment);

#endif
