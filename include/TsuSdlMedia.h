#ifndef __H_TSU_SDL_MEDIA_H_
#define __H_TSU_SDL_MEDIA_H_

#include <SDL.h>

typedef struct {
    int w, h;
} Dimensions;

typedef struct {
    SDL_Window* window;
    Dimensions dim;
    SDL_Renderer* renderer;
    SDL_Texture * texture;
} TsuSdlMedia;


#endif

