#ifndef __H_TSU_ALLOC_H_
#define __H_TSU_ALLOC_H_

#include <stddef.h>

typedef struct {
    void* (*alloc)(size_t n);
    void (*dealloc)(void*);
} TsuAlloc;

TsuAlloc* tsuAllocator();

#endif

