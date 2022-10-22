#ifndef __H_TSU_PLANAR_H_
#define __H_TSU_PLANAR_H_

#include <SDL.h>
#include <stddef.h>

#include "TsuBoard.h"
#include "TsuSdlMedia.h"
#include "TsuPencil.h"
#include "TsuMouse.h"
#include "TsuNodes.h"

typedef enum { GameStateTitle, GameStatePlaying, GameStateEnded } GameState;

typedef struct TsuPlanarGame TsuPlanarGame;

typedef struct {
    int (*process_input) (TsuPlanarGame*);
    int (*update) (TsuPlanarGame*);
    //int (*render) (TsuPlanarGame*);
} GameMethods;

typedef struct TsuPlanarGame{
    TsuBoard board;
    TsuSdlMedia media;
    TsuPencil pencil;
    TsuMouse mouse;
    bool keep_running;
    TsuNodes nodes;
    GameState state;
    GameMethods methods;
} TsuPlanarGame;


int init_planar_game(TsuPlanarGame* g, size_t w, size_t h);
void destroy_planar_game(TsuPlanarGame* g);

#endif

