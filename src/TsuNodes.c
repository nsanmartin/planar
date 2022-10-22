#include <stdlib.h>
#include <stdio.h>
#include "TsuNodes.h"

#define NODES_INITIAL_CAPACITY 32

#include "TsuLambda.h"
#include "TsuAlloc.h"
#include "TsuPair.h"
#include "TsuPlanarGame.h"

void nodes_clear(TsuNodes* nodes) {
    nodes->sz = 0;
}

int init_nodes(TsuNodes* nodes) {
    TsuNode* ps = tsu_malloc(NODES_INITIAL_CAPACITY * sizeof(TsuNode));
    if (!ps) {
        return -1;
    }

    *nodes = (TsuNodes) {
        .sz = 0,
        .capacity = NODES_INITIAL_CAPACITY,
        .ps = ps,
        .node_size = 18, // defaut
    };

    return 0;
}

void destroy_nodes(TsuNodes* nodes) {
    tsu_free(nodes->ps);
}

int nodes_push_back(TsuNodes* ns, Point p) {
    if (ns->sz >= ns->capacity) {
        ns->ps = tsu_realloc(ns->ps, 2 * ns->capacity * sizeof(Point));
        if (!ns->ps) {
            return -1;
        }

        ns->capacity *= 2;
    }
    ns->ps[ns->sz++] = (TsuNode){ .p=p, .touched = false };
    return 0;
}

int nodes_append_rand(TsuPlanarGame* g, size_t n) {
    size_t nsz = g->nodes.node_size;
    //todo: check distance between nodes
    while (n--) {
        int random = rand();
        Point p = (Point) {
            random % (g->board.w - 2*nsz) + nsz, random % (g->board.h - 2*nsz) + nsz
        };

        int error = nodes_push_back(&g->nodes, p);
        if (error) {
            return error;
        }
    }
    return 0;
}

int nodes_for_each(LamConsumer* lam, void* beg, void* end, LamPredicate* pred) {
    int error = 0;

    for (TsuNode* it = beg; it < (TsuNode*)end; ++it) {
        if (pred) {
            error = pred->app(pred, it);
            if (error) { return error; }
            if(!pred->test) { continue; }
        }

        error = lam->app(lam, it);
        if (error) { return error; }

    }
    return 0;
}

int nodes_accum(Lambda* lam, void* beg, void* end, LamPredicate* pred, int* accum) {
    int error = 0;

    for (TsuNode* it = beg; it < (TsuNode*)end; ++it) {
        if (pred) {
            error = pred->app(pred, it);
            if (error) { return error; }
            if(!pred->test) { continue; }
        }

        error = lam->app(lam, it);
        if (error) { return error; }
        *accum += *(int*)lam->value;

    }
    return 0;
}

//todo: use |n - (x,y)| < r
bool node_contains_point(Point* n, int sz, int x, int y) {
    return x >= n->x - sz && x <= n->x + sz
        && y >= n->y - sz && y <= n->y + sz;
}

struct LamCtxNodeContainsPoint { int sz, x, y; };

int accum_node_touched(Lambda* lam, void* arg) {
    *(int*)lam->value = ((TsuNode*)arg)->touched ? 1 : 0;
    return 0;
}

int nodes_count_touched(const TsuPlanarGame* g, int* res) {

    *res = 0;
    int value = 0;
    Lambda lam = { .app = accum_node_touched, .value=&value};
    int error = nodes_accum(&lam, g->nodes.ps, g->nodes.ps+g->nodes.sz, NULL, res);
    return error;
}

int lam_node_contains_point(LamPredicate* lam, void* data) {
    Point* node = (Point*) data;
    struct LamCtxNodeContainsPoint* ctx = lam->ctx;
    lam->test = node_contains_point(node, ctx->sz, ctx->x, ctx->y);
    return 0;
}


int nodes_find(LamPredicate * lam, const TsuNodes* ns, TsuNode** res) {
    TsuNode* end = ns->ps + ns->sz;
    for (*res = ns->ps; *res < end; ++*res) {
        int error = lam->app(lam, *res);
        if (error) {
            return error;
        }
        if (lam->test) {
            return 0;
        }
    }
    return 0;
}

int nodes_find_touched(const TsuNodes* ns, int x, int y, TsuNode** res) {

    struct LamCtxNodeContainsPoint ctx = { .sz=ns->node_size, .x=x, .y=y };
    LamPredicate lam = { .app=lam_node_contains_point, .ctx=(void*)&ctx, .test=false };
    return nodes_find(&lam, ns, res);

}

