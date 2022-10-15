#include <stdbool.h>
#include <stddef.h>

#include "TsuDraw.h"
#include "TsuPencil.h"
#include "TsuBoard.h"


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
                    *ptr = 1;
                } else {
                    fprintf(stderr, "invalid board pos\n");
                }
            }
            
        }
    }
}
