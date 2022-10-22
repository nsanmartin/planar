#include <assert.h>

#include "TsuBoard.h"

int init_board(TsuBoard* board, size_t w, size_t h) {
    Uint32* data = tsu_malloc(w * h * sizeof(Uint32));
    if (!data) { return -1; }
    memset(data, 255, w * h * sizeof(Uint32));
    *board = (TsuBoard) {
        .data = data,
        .w = w,
        .h = h,
    };

    return 0;
}

void destroy_board(TsuBoard* board) {
    tsu_free(board->data);
}

//todo: parametrize colors
TsuBoard* newTsuBoard(size_t w, size_t h) {
    Uint32* data = tsu_malloc(w * h * sizeof(Uint32));
    if (!data) { return NULL; }
    memset(data, 255, w * h * sizeof(Uint32));

    TsuBoard* rv = tsu_malloc(sizeof(TsuBoard));
    if (!rv) {
        tsu_free(data);
        return NULL;
    }

    *rv = (TsuBoard) {
        .data = data,
        .w = w,
        .h = h,
    };
    return rv;
}

void freeTsuBoard(TsuBoard* board) {
    tsu_free(board->data);
    tsu_free(board);
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
