#include <SDL.h>

#include <TsuPlanarGame.h>
#include "TsuAlloc.h"
#include "TsuDraw.h"

SortedPair sortedPairFrom(int a, int b);
Line compute_line(int x0, int x1, int y0, int y1);
void tsu_draw_dot(int x, int y, TsuBoard* t, const TsuPencil* pencil);
void tsu_draw_node(TsuBoard* t, Point p, int sz);
TsuNodes* sample_nodes();

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

    //todo: move this
    for (int i = 0; i < rv->nodes->sz; ++i) {
        tsu_draw_node(rv->board, rv->nodes->ps[i], rv->nodes->node_size);
    }

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



int process_input(TsuPlanarGame* game) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
            game->keep_running = false;
            return 0;
        }

        int x, y;
        bool left_button = SDL_GetMouseState( &x, &y ) & SDL_BUTTON_LEFT;

        if (left_button && e.type == SDL_MOUSEBUTTONDOWN) {
            game->mouse.is_up = false;
            game->mouse.x = x;
            game->mouse.y = y;

            if (y >= 0 && y < game->media->dim.h && x >= 0 && x < game->media->dim.w) {
                tsu_draw_dot(x, y, game->board, &game->pencil);
            }

        } else if (!game->mouse.is_up && e.type == SDL_MOUSEMOTION) {
            if (abs(game->mouse.x-x) > abs(game->mouse.y-y)) {
                Line l = compute_line(game->mouse.x, x, game->mouse.y, y);
                SortedPair xs = sortedPairFrom(game->mouse.x, x);
                for (int i = xs.min; i < xs.max; ++i) {
                    int j = l.yintercept + l.slope * i;
                    tsu_draw_dot(i, j, game->board, &game->pencil);
                }
            } else {
                SortedPair ys = sortedPairFrom(game->mouse.y, y);
                if (game->mouse.x == x) {
                    for (int i = ys.min; i < ys.max; ++i) {
                        tsu_draw_dot(x, i, game->board, &game->pencil);
                    }
                } else {
                    Line l = compute_line(game->mouse.x, x, game->mouse.y, y);
                    for (int i = ys.min; i < ys.max; ++i) {
                        int j = (i - l.yintercept) / l.slope;
                        tsu_draw_dot(j, i, game->board, &game->pencil);
                    }
                }
            }
            game->mouse.x = x;
            game->mouse.y = y;

        } 
        if (e.type == SDL_MOUSEBUTTONUP ) {
            game->mouse.is_up = true;
        }
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
