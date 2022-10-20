#include <SDL.h>

#include <TsuPlanarGame.h>
#include "TsuAlloc.h"
#include "TsuDraw.h"

SortedPair sortedPairFrom(int a, int b);
Line compute_line(int x0, int x1, int y0, int y1);
void tsu_draw_dot(int x, int y, TsuBoard* t, const TsuPencil* pencil);
void tsu_draw_node(TsuBoard* t, Point p, int sz);
TsuNodes* sample_nodes();
int tsu_draw_points(TsuPlanarGame* g);

TsuPlanarGame* newPlanarGameWith(size_t w, size_t h) {

    TsuBoard* board = newTsuBoard(w, h);
    if (!board) { return NULL; }
    TsuSdlMedia* media = newTsuSdlMedia((Dimensions){ .w = w, .h = h});
    if (!media) {
        freeTsuBoard(board);
        return NULL;
    }

    TsuNodes* nodes = sample_nodes();
    if (!nodes) {
        freeTsuBoard(board);
        freeTsuSdlMedia(media);
        return NULL;
    }

    TsuPlanarGame* rv = tsu_malloc(sizeof(TsuPlanarGame));
    if (!rv) {
        freeNodes(nodes);
        freeTsuBoard(board);
        freeTsuSdlMedia(media);
        return NULL;
    }

    *rv = (TsuPlanarGame) {
        .board = board,
        .media = media,
        .pencil = { .sz = 4 },
        .mouse = { .is_up = true, .x = 0, .y = 0 },
        .keep_running = true,
        .nodes = nodes
    };

    tsu_draw_points(rv);

    return rv;
}

void freePlanarGame(TsuPlanarGame* app) {
    freeTsuBoard(app->board);
    freeTsuSdlMedia(app->media);
    tsu_free(app);
}

int update(TsuPlanarGame* game) {
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
    SDL_UpdateTexture(game->media->texture, NULL, game->board->data, 640 * sizeof(Uint32));
    return 0;
}



void render(TsuPlanarGame* app) {

    //todo: is this needed?
    //SDL_RenderClear(app->media->renderer);
    SDL_RenderCopy(app->media->renderer, app->media->texture, NULL, NULL);
    SDL_RenderPresent(app->media->renderer);
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

        int x, y;
        bool left_button = SDL_GetMouseState( &x, &y ) & SDL_BUTTON_LEFT;

        if (left_button && e.type == SDL_MOUSEBUTTONDOWN) {

            if (tsu_point_in_window(game->media, x, y)) {
                tsu_draw_dot(x, y, game->board, &game->pencil);
            }
            game->mouse.is_up = false;

        } else if (!game->mouse.is_up && e.type == SDL_MOUSEMOTION) {
            if (tsu_delta_x_greater_than_delta_y(&game->mouse, x, y)) {
                SortedPair xs = sortedPairFrom(game->mouse.x, x);
                Line l = compute_line(game->mouse.x, x, game->mouse.y, y);
                for (int i = xs.min; i < xs.max; ++i) {
                    int j = l.yintercept + l.slope * i;
                    tsu_draw_dot(i, j, game->board, &game->pencil);
                }
            } else {
                SortedPair ys = sortedPairFrom(game->mouse.y, y);
                Line l = compute_line(game->mouse.y, y, game->mouse.x, x);
                for (int i = ys.min; i < ys.max; ++i) {
                    int j = l.yintercept + l.slope * i;
                    tsu_draw_dot(j, i, game->board, &game->pencil);
                }
            }

        } 
        if (e.type == SDL_MOUSEBUTTONUP ) {
            game->mouse.is_up = true;
        }

        game->mouse.x = x;
        game->mouse.y = y;
    }

    return 0;
}

bool keep_running(const TsuPlanarGame* game) { return game->keep_running; }



TsuNodes* sample_nodes() {
    TsuNodes* rv = newNodes();
    if (!rv) { return NULL; }
    int error = 0;
    error = nodes_push_back(rv, (Point){10, 10});
    if (error) {
        freeNodes(rv);
        return NULL;
    }
    error = nodes_push_back(rv, (Point){100, 190});
    if (error) {
        freeNodes(rv);
        return NULL;
    }
    return rv;
}

