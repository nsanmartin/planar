#include <assert.h>

#include "TsuBoard.h"

void reset_board(TsuBoard* board) {
    memset(board->data, 255, board->w * board->h * sizeof(Uint32));
}

int init_board(TsuBoard* board, size_t w, size_t h) {
    Uint32* data = tsu_malloc(w * h * sizeof(Uint32));
    if (!data) { return -1; }
    *board = (TsuBoard) {
        .data = data,
        .w = w,
        .h = h,
    };

    reset_board(board);
    return 0;
}

void destroy_board(TsuBoard* board) {
    tsu_free(board->data);
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
