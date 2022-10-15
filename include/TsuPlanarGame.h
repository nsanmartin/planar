#ifndef __H_TSU_PLANAR_H_
#define __H_TSU_PLANAR_H_

#include <SDL.h>
#include <stddef.h>

#include "TsuBoard.h"
#include "TsuSdlMedia.h"
#include "TsuPencil.h"
#include "TsuMouse.h"

typedef struct {
    TsuBoard* board;
    TsuSdlMedia* media;
    TsuPencil pencil;
    TsuMouse mouse;
    bool keep_running;
} TsuPlanarGame;

TsuPlanarGame* newPlanarGameWith(size_t w, size_t h);
void freePlanarGame(TsuPlanarGame* app);

#endif

