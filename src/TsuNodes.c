#include <stdio.h>
#include "TsuNodes.h"

#define NODES_INITIAL_CAPACITY 32

#include "TsuAlloc.h"

TsuNodes* newNodes() {
    Point* ps = tsu_malloc(NODES_INITIAL_CAPACITY * sizeof(Point));
    if (!ps) {
        return NULL;
    }

    TsuNodes* rv = tsu_malloc(sizeof(TsuNodes));
    if (!rv) {
        tsu_free(ps);
        NULL;
    }

    *rv = (TsuNodes) {
        .sz = 0,
        .capacity = NODES_INITIAL_CAPACITY,
        .ps = ps
    };

    return rv;
}

void freeNodes(TsuNodes* ns) {
    tsu_free(ns->ps);
    tsu_free(ns);
}

// int nodes_push_back(TsuNodes* ns, Point p) {
// 
// }
