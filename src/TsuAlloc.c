#include "TsuAlloc.h"

// TsuAlloc tsuAlloc = {
//     .alloc = malloc,
//     .dealloc = free
// };

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

TsuAlloc* tsuAllocator() {
    return &tsuAlloc;
}
