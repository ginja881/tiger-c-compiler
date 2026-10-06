#include "alloc.h"


void* checked_malloc(size_t bytes) {
   void* newData = malloc(bytes);
   assert(newData);
   return newData;
}
Arena Arena_makeArena(size_t capacity, Arena next) {
   Arena newArena = (Arena)checked_malloc(sizeof(struct Arena_));
   newArena->buffer = (unsigned char*)checked_malloc(capacity);
   newArena->bufferCapacity = capacity;
   newArena->allocatedBytes = 0;
   newArena->next = NULL;
   return newArena;
}

void Arena_freeArena(Arena arenaHead) {
    Arena currentArena = arenaHead;
    while (currentArena != NULL) {
       Arena oldArena = currentArena;
       currentArena = currentArena->next;

       free(oldArena->buffer);
       free(oldArena);
    }

}

void* Arena_allocArena(Arena* arena, size_t dataSize, size_t alignment) {
    if (arena == NULL) 
       return NULL;
    
    size_t paddingBytes = (alignment - (dataSize % alignment)) % alignment;
    size_t overallSize = paddingBytes + dataSize;

    if (overallSize > DEFAULT_ARENA_CAPACITY) {
        (*arena) = Arena_makeArena(overallSize, (*arena));
	(*arena)->allocatedBytes = overallSize;
	return (*arena)->buffer;
    }

    
    if (dataSize > ((*arena)->bufferCapacity - (*arena)->allocatedBytes)) {
         (*arena) = Arena_makeArena(DEFAULT_ARENA_CAPACITY, (*arena));
	 (*arena)->allocatedBytes = overallSize;
	 return (*arena)->buffer;
    }
    
    void* data = (*arena)->buffer + (*arena)->allocatedBytes;
    (*arena)->allocatedBytes += overallSize;

    return data;
}
