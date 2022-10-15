#ifndef __H_TSU_BOARD_H_ 
#define __H_TSU_BOARD_H_ 

#include <SDL.h>

#include <TsuPoint.h>
#include "TsuPencil.h"
#include "TsuAlloc.h"


typedef struct TsuBoard {
    Uint32* data;
    size_t w,h;
    TsuAlloc* alloc;
} TsuBoard;


TsuBoard* newTsuBoard(TsuAlloc* alloc, size_t w, size_t h); 
void freeTsuBoard(TsuBoard* board);

Uint32* tsuBoardAt(TsuBoard* board, int x, int y);
#endif
