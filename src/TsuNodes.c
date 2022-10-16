#include <stdio.h>
#include "TsuNodes.h"

#define NODES_INITIAL_CAPACITY 32

#include "TsuLambda.h"
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
        .ps = ps,
        .node_size = 18, // defaut
        .touched = false
    };

    return rv;
}

void freeNodes(TsuNodes* ns) {
    tsu_free(ns->ps);
    tsu_free(ns);
}

int nodes_push_back(TsuNodes* ns, Point p) {
    if (ns->sz >= ns->capacity) {
        ns->ps = tsu_realloc(ns->ps, 2 * ns->capacity * sizeof(Point));
        if (!ns->ps) {
            return -1;
        }

        ns->capacity *= 2;
    }
    ns->ps[ns->sz++] = p;
    return 0;
}

int nodes_for_each(LamConsumer* lam, TsuNodes* ns) {
    Point* end = ns->ps + ns->sz;

    for (Point* it = ns->ps; it < end; ++it) {
        int error = lam->app(lam, it);
        if (error) {
            return error;
        }
    }
    return 0;
}

