#include <assert.h>

#include "TsuBoard.h"


TsuBoard tsu_board = {0};

TsuBoard* newTsuBoard(TsuAlloc* tsu_alloc, size_t w, size_t h) {
    Uint32* data = tsu_alloc->alloc(w * h * sizeof(Uint32));
    if (!data) { return NULL; }
    memset(data, 255, w * h * sizeof(Uint32));

    tsu_board = (TsuBoard) {
        .data = data,
        .w = w,
        .h = h,
        .alloc = tsu_alloc
    };
    return &tsu_board;
}

void freeTsuBoard(TsuBoard* board) {
    if (board) {
        board->alloc->dealloc(board->data);
    }
}

Uint32* tsuBoardAt(TsuBoard* t, int x, int y) {
    size_t ix = t->w * y + x;
    if (ix >= t->w * t->h) {
        printf("index error: %d, %d\n", x, y);
    }
    if (ix < t->w * t->h) {
        return &t->data[ix];
    }
    return NULL;
}
