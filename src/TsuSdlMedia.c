#include "SDL.h"
#include "TsuPoint.h"

#include "TsuAlloc.h"
#include "TsuSdlMedia.h"


int sdl_media_init(TsuSdlMedia* media) {
    
    if (SDL_Init (SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "Couldn't initialize SDL\n");
        return -1;
    }

    atexit (SDL_Quit);

    media->window = SDL_CreateWindow(
        "planar",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        media->dim.w,
        media->dim.h,
        SDL_WINDOW_SHOWN
    );
    
    if (media->window == NULL) {
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        return -1;
    }
    
    media->renderer = SDL_CreateRenderer(
        media->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    if (media->renderer == NULL) {
        SDL_DestroyWindow(media->window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return -1;
    }

    media->texture = SDL_CreateTexture(
        media->renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC,
        media->dim.w,
        media->dim.h
    );

    if (media->texture == NULL) {
        SDL_DestroyWindow(media->window);
        SDL_DestroyRenderer(media->renderer);
        fprintf(stderr, "SDL_CreateTexture Error: %s\n", SDL_GetError());
        return -1;
    }
    return 0;
}

int init_sdl_media(TsuSdlMedia* media, size_t w, size_t h) {
    media->dim = (Dimensions){.w=w, .h=h};
    return sdl_media_init(media);
}

void destroy_sdl_media(TsuSdlMedia* media) {
    SDL_DestroyTexture(media->texture);
    SDL_DestroyRenderer(media->renderer);
    SDL_DestroyWindow(media->window); 
}

