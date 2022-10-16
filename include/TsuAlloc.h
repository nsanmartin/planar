#ifndef __H_TSU_ALLOC_H_
#define __H_TSU_ALLOC_H_

#include <stddef.h>

typedef struct {
    void* (*alloc)(size_t n);
    void (*dealloc)(void*);
} TsuAlloc;

TsuAlloc* tsu_allocator();

void* tsu_malloc(size_t n);
void tsu_free(void* ptr);

#endif

