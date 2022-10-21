#include <stdio.h>
#include "TsuNodes.h"

#define NODES_INITIAL_CAPACITY 32

#include "TsuLambda.h"
#include "TsuAlloc.h"

TsuNodes* newNodes() {
    TsuNode* ps = tsu_malloc(NODES_INITIAL_CAPACITY * sizeof(TsuNode));
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
    ns->ps[ns->sz++] = (TsuNode){ .p=p, .touched = false};
    return 0;
}

int nodes_for_each(LamConsumer* lam, TsuNodes* ns) {
    TsuNode* end = ns->ps + ns->sz;

    for (TsuNode* it = ns->ps; it < end; ++it) {
        int error = lam->app(lam, it);
        if (error) {
            return error;
        }
    }
    return 0;
}

//todo: use |n - (x,y)| < r
bool node_contains_point(Point* n, int sz, int x, int y) {
    return x >= n->x - sz && x <= n->x + sz
        && y >= n->y - sz && y <= n->y + sz;
}

struct LamNodeContainsPointCtx { int sz, x, y; };

int lam_node_contains_point(LamPredicate* lam, void* data) {
    Point* node = (Point*) data;
    struct LamNodeContainsPointCtx* ctx = (struct LamNodeContainsPointCtx*) lam->ctx;
    lam->pred = node_contains_point(node, ctx->sz, ctx->x, ctx->y);
    return 0;
}


int nodes_find(LamPredicate * lam, const TsuNodes* ns, TsuNode** res) {
    TsuNode* end = ns->ps + ns->sz;
    for (*res = ns->ps; *res < end; ++*res) {
        int error = lam->app(lam, *res);
        if (error) {
            return error;
        }
        if (lam->pred) {
            return 0;
        }
    }
    return 0;
}

int nodes_find_touched(const TsuNodes* ns, int x, int y, TsuNode** res) {

    struct LamNodeContainsPointCtx ctx = { .sz=ns->node_size, .x=x, .y=y };
    LamPredicate lam = { .app=lam_node_contains_point, .ctx=(void*)&ctx, .pred=false };
    return nodes_find(&lam, ns, res);

}

