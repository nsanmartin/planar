#include <time.h>
#include <SDL.h>

#include <TsuPlanarGame.h>
#include "TsuAlloc.h"
#include "TsuDraw.h"

void lam_draw_node(TsuBoard* t, Point p, int sz);
TsuNodes* sample_nodes();
int init_sdl_media(TsuSdlMedia* media, int w, int h);
int init_board(TsuBoard* board, size_t w, size_t h);
void destroy_board(TsuBoard* board);
int init_nodes(TsuNodes* nodes);
void destroy_nodes(TsuNodes* nodes);
void destroy_sdl_media(TsuSdlMedia* media);
void reset_board(TsuBoard* board);
void nodes_clear(TsuNodes* nodes);
int tsu_draw_points(TsuPlanarGame* g);
int set_game_state(TsuPlanarGame* g, GameState state);


int reset_planar_game(TsuPlanarGame* g) {
    reset_board(&g->board);

    nodes_clear(&g->nodes);
    return 0;

}

int init_planar_game(TsuPlanarGame* g, size_t w, size_t h) {
   srand(time(NULL));
   int error = init_board(&g->board, w, h); 
   if (error) {
       return error;
   }

   error = init_sdl_media(&g->media, w, h);
   if (error) {
       destroy_board(&g->board);
       return error;
   }

   error = init_nodes(&g->nodes);
   if (error) {
       destroy_sdl_media(&g->media);
       destroy_board(&g->board);
       return error;
   }


   g->keep_running = true;
   g->mouse = (TsuMouse) { .is_up = true, .x = 0, .y = 0 };
   g->pencil = (TsuPencil) { .sz = 4 };
   g->state = GameStateTitle;
   set_game_state(g, GameStateTitle);

    return error;
}

void destroy_planar_game(TsuPlanarGame* g) {
   destroy_sdl_media(&g->media);
   destroy_board(&g->board);
   destroy_nodes(&g->nodes);
}




void render(TsuPlanarGame* app) {

    //todo: is this needed?
    //SDL_RenderClear(app->media->renderer);
    SDL_RenderCopy(app->media.renderer, app->media.texture, NULL, NULL);
    SDL_RenderPresent(app->media.renderer);
}


bool tsu_sdl_quit_event(const SDL_Event* e) {
    return e->type == SDL_QUIT || (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_q);
}

bool tsu_point_in_window(const TsuSdlMedia* m, int x, int y) {
    return y >= 0 && y < m->dim.h && x >= 0 && x < m->dim.w;
}

bool tsu_delta_x_greater_than_delta_y(const TsuMouse* m, int x, int y) {
    return abs(m->x-x) > abs(m->y-y);
}


bool keep_running(const TsuPlanarGame* game) { return game->keep_running; }

