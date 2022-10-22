#include <stdbool.h>
#include <stddef.h>

#include "TsuPoint.h"
#include "TsuDraw.h"
#include "TsuPencil.h"
#include "TsuBoard.h"
#include "TsuPlanarGame.h"
#include "TsuLambda.h"
#include "TsuPair.h"


int nodes_for_each(LamConsumer* lam, Pair* pair);


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

int tsu_draw_node(LamConsumer* lam, void* params) {
    if (!lam || !lam->ctx || !params) { return -1; }
    TsuPlanarGame* g = (TsuPlanarGame*) lam->ctx;
    int sz = g->nodes.node_size;
    Pair* pair = params;
    TsuNode* n = (TsuNode*) pair->first;
    uint32_t* pcolor = pair->second;

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
    LamConsumer lam = { .app = tsu_draw_node, .ctx = g, };
    uint32_t color = 0x70e4;
    Pair pair = { .first=&g->nodes, .second=&color};
    int error = nodes_for_each(&lam, &pair);
    return error;
}

int tsu_draw_node_if_touched(LamConsumer* lam, void* params) {

    Pair* pair = params;
    TsuNode* n = pair->first;

    if (n->touched) {
        uint32_t color = 0x11a475;
        Pair pair = { .first=n, .second=&color};
        return tsu_draw_node(lam, &pair);
    }
    return 0;
}


int tsu_draw_touched_points(TsuPlanarGame* g) {
    LamConsumer lam = { .app = tsu_draw_node_if_touched , .ctx = g, };

    uint32_t color = 0x179e4;
    Pair pair = { .first=&g->nodes, .second=&color};
    int error = nodes_for_each(&lam, &pair);
    return error;
}

