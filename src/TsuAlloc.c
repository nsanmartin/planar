#include <stdlib.h>
#include "TsuAlloc.h"

// #define TSU_MEM_TEST
#ifdef TSU_MEM_TEST
#include <stdio.h>
#define MAX_MALLOC_TIMES 1
int tsu_malloc_times = MAX_MALLOC_TIMES;
int tsu_malloc_calls = 0;
int tsu_free_calls = 0;

void* tsu_malloc(size_t n) {
    if (tsu_malloc_times > 0) {
        printf("malloc calls: %d. Remaining: %d\n", ++tsu_malloc_calls, --tsu_malloc_times);
        return malloc(n);
    }
    return NULL;
}

void tsu_free(void* ptr) {
    ++tsu_free_calls;
    printf("free calls: %d (malloc-free: %d)\n", tsu_free_calls, tsu_malloc_calls - tsu_free_calls);
    free(ptr);
}

void* tsu_realloc(void* ptr, size_t size) {
    if (tsu_malloc_times > 0) {
        printf("realloc call. Remaining: %d\n", tsu_malloc_times--);
        return realloc(ptr, size);
    }
    return NULL;
}
#else
void* tsu_malloc(size_t n) { return malloc(n); }
void tsu_free(void* ptr) { free(ptr); }
void* tsu_realloc(void* ptr, size_t size) { return realloc(ptr, size); }
#endif

#define TSU_PLANAR_MEM_SZ 1600000

unsigned char tsu_planar_memory[TSU_PLANAR_MEM_SZ] = {0};
unsigned char* tsu_planar_free_mem = tsu_planar_memory;

void* tsu_static_alloc(size_t n) {
    if (tsu_planar_free_mem + n > tsu_planar_memory + TSU_PLANAR_MEM_SZ) {
        return NULL;
    }
    void* rv = tsu_planar_free_mem;
    tsu_planar_free_mem += n;
    return rv;
}

void tsu_static_free(void* ptr) {
    if (ptr) {
    }
    return;
}

TsuAlloc tsuAlloc = {
    .alloc = tsu_static_alloc,
    .dealloc = tsu_static_free
};

TsuAlloc* tsu_allocator() {
    return &tsuAlloc;
}
