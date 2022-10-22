#include <time.h>
#include <SDL.h>

#include <TsuPlanarGame.h>
#include "TsuAlloc.h"
#include "TsuDraw.h"

SortedPair sortedPairFrom(int a, int b);
Line compute_line(int x0, int x1, int y0, int y1);
void tsu_draw_dot(int x, int y, TsuBoard* t, const TsuPencil* pencil);
void lam_draw_node(TsuBoard* t, Point p, int sz);
TsuNodes* sample_nodes();
int tsu_draw_points(TsuPlanarGame* g);
int tsu_draw_touched_points(TsuPlanarGame* g);
int init_sdl_media(TsuSdlMedia* media, int w, int h);
int init_board(TsuBoard* board, size_t w, size_t h);
void destroy_board(TsuBoard* board);
int init_nodes(TsuNodes* nodes);
void destroy_nodes(TsuNodes* nodes);
void destroy_sdl_media(TsuSdlMedia* media);
int nodes_append_rand(TsuPlanarGame* g, size_t n);

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

   error = nodes_append_rand(g, 3);

   g->keep_running = true;
   g->mouse = (TsuMouse) { .is_up = true, .x = 0, .y = 0 };
   g->pencil = (TsuPencil) { .sz = 4 };

   //todo: take outside
    error = tsu_draw_points(g);
    return error;
}

void destroy_planar_game(TsuPlanarGame* g) {
   destroy_sdl_media(&g->media);
   destroy_board(&g->board);
   destroy_nodes(&g->nodes);
}

int update(TsuPlanarGame* game) {
    tsu_draw_touched_points(game);
    //todo: move to update & check this docs:
    /**
     * this is a fairly slow function, intended for use with static textures that
     * do not change often.
     *
     * if the texture is intended to be updated often, it is preferred to create
     * the texture as streaming and use the locking functions referenced below.
     * while this function will work with streaming textures, for optimization
     * reasons you may not get the pixels back if you lock the texture afterward.
     */
    SDL_UpdateTexture(game->media.texture, NULL, game->board.data, 640 * sizeof(Uint32));
    return 0;
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

int process_input(TsuPlanarGame* game) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (tsu_sdl_quit_event(&e)) {
            game->keep_running = false;
            return 0;
        }
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_n) {
            //todo: next graph
            game->keep_running = false;
            return 0;
        }

        int x, y;
        bool left_button = SDL_GetMouseState( &x, &y ) & SDL_BUTTON_LEFT;

        if (left_button && e.type == SDL_MOUSEBUTTONDOWN) {

            if (tsu_point_in_window(&game->media, x, y)) {
                tsu_draw_dot(x, y, &game->board, &game->pencil);
            }
            game->mouse.is_up = false;

        } else if (!game->mouse.is_up && e.type == SDL_MOUSEMOTION) {
            if (tsu_delta_x_greater_than_delta_y(&game->mouse, x, y)) {
                SortedPair xs = sortedPairFrom(game->mouse.x, x);
                Line l = compute_line(game->mouse.x, x, game->mouse.y, y);
                for (int i = xs.min; i < xs.max; ++i) {
                    int j = l.yintercept + l.slope * i;
                    tsu_draw_dot(i, j, &game->board, &game->pencil);
                }
            } else {
                SortedPair ys = sortedPairFrom(game->mouse.y, y);
                Line l = compute_line(game->mouse.y, y, game->mouse.x, x);
                for (int i = ys.min; i < ys.max; ++i) {
                    int j = l.yintercept + l.slope * i;
                    tsu_draw_dot(j, i, &game->board, &game->pencil);
                }
            }

        } 
        if (e.type == SDL_MOUSEBUTTONUP ) {
            game->mouse.is_up = true;
        }

        game->mouse.x = x;
        game->mouse.y = y;

        if (!game->mouse.is_up) {
            TsuNode* touched_node = NULL;
            int error = nodes_find_touched(&game->nodes, x, y, &touched_node);
            if (error) {
                return error;
            }
            if (touched_node != NULL && touched_node != game->nodes.sz + game->nodes.ps) {
                touched_node->touched = true;
            }
        }

    }


    return 0;
}

bool keep_running(const TsuPlanarGame* game) { return game->keep_running; }

