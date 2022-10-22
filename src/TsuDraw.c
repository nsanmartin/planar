#include <stdbool.h>
#include <stddef.h>

#include "TsuPoint.h"
#include "TsuDraw.h"
#include "TsuPencil.h"
#include "TsuBoard.h"
#include "TsuPlanarGame.h"
#include "TsuLambda.h"
#include "TsuPair.h"


int nodes_for_each(LamConsumer* lam, void* beg, void* end, LamPredicate* pred);


SortedPair sortedPairFrom(int a, int b) {
    if (a <= b) {
        return (SortedPair) { .min = a, .max = b };
    } else {
        return (SortedPair) { .min = b, .max = a };
    }
}

Line compute_line(int x0, int x1, int y0, int y1) {
    Line rv;
    rv.slope = (double)(y1-y0) / (double)(x1-x0);
    rv.yintercept = y0 - rv.slope*x0;
    return rv;
}

//todo: parametrize colors
void tsu_draw_dot(int x, int y, TsuBoard* t, const TsuPencil* pencil) {
    size_t sz = pencil->sz;
    size_t fromx = x - sz;
    size_t fromy = y - sz;
    size_t tox = x + sz - 1;
    size_t toy = y + sz - 1;

    if (fromx < t->w && tox < t->w && fromy < t->h && toy < t->h) {
        for (size_t i = fromx; i < tox; ++i) {
            for (size_t j = fromy; j < toy; ++j) {
                Uint32* ptr = tsuBoardAt(t, i, j);
                if (ptr) {
                    *ptr = 0;
                } else {
                    fprintf(stderr, "invalid board pos\n");
                }
            }
            
        }
    }
}

int lam_draw_node(LamConsumer* lam, void* arg) {
    if (!lam || !lam->ctx || !arg) { return -1; }
    Pair* ctx = lam->ctx;
    TsuPlanarGame* g = ctx->first;
    uint32_t* pcolor = ctx->second;
    int sz = g->nodes.node_size;
    TsuNode* n = arg;

    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j) {
            Uint32* ptr = tsuBoardAt(&g->board, n->p.x + i, n->p.y + j);
            if (ptr) {
                *ptr = *pcolor;
            }
        }
    }

    return 0;
}

int tsu_draw_points(TsuPlanarGame* g) {
    uint32_t color = 0x5da7d8;
    Pair ctx = { .first = g, .second = &color };
    LamConsumer lam = { .app = lam_draw_node, .ctx = &ctx };
    int error = nodes_for_each(&lam, g->nodes.ps, g->nodes.ps+g->nodes.sz, NULL);
    return error;
}

int node_is_touched(LamPredicate* pred, void* n) {
    pred->test = ((TsuNode*)n)->touched;
    return 0;
}

int tsu_draw_touched_points(TsuPlanarGame* g) {
    uint32_t color = 0x749f82;
    Pair ctx = { .first=g, .second=&color};
    LamConsumer lam = { .app = lam_draw_node , .ctx = &ctx, };

    LamPredicate pred = { .app=node_is_touched, .test = false };
    int error = nodes_for_each(&lam, g->nodes.ps, g->nodes.ps+g->nodes.sz, &pred);
    return error;
}


