#include <TsuPlanarGame.h>
#include "TsuDraw.h"

int tsu_draw_touched_points(TsuPlanarGame* g);
int nodes_count_touched(const TsuPlanarGame* g, int* res);
int reset_planar_game(TsuPlanarGame* g);
bool tsu_sdl_quit_event(const SDL_Event* e);
bool tsu_point_in_window(const TsuSdlMedia* m, int x, int y);
void tsu_draw_dot(int x, int y, TsuBoard* t, const TsuPencil* pencil);
bool tsu_delta_x_greater_than_delta_y(const TsuMouse* m, int x, int y);
SortedPair sortedPairFrom(int a, int b);
Line compute_line(int x0, int x1, int y0, int y1);
int nodes_find_touched(const TsuNodes* ns, int x, int y, TsuNode** res);
int tsu_draw_points(TsuPlanarGame* g);
int nodes_append_rand(TsuPlanarGame* g, size_t n);

int set_game_state(TsuPlanarGame* g, GameState state);

int update_state_title(TsuPlanarGame* game) {
    SDL_UpdateTexture(game->media.texture, NULL, game->board.data, 640 * sizeof(Uint32));
    return 0;
}

int process_input_state_title(TsuPlanarGame* game) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (tsu_sdl_quit_event(&e)) {
            game->keep_running = false;
            game->state = GameStateEnded;
            return set_game_state(game, GameStateEnded);
        }

        if ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_n) || e.type == SDL_MOUSEBUTTONDOWN) {
            return set_game_state(game, GameStatePlaying);
        }
    }
    return  0;
}

int update_state_playing(TsuPlanarGame* game) {
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

    int touched_nodes_count = 0;
    nodes_count_touched(game, &touched_nodes_count);
    if (touched_nodes_count == game->nodes.sz) {
        reset_planar_game(game);
        int error = set_game_state(game, GameStateTitle);
        if (error) {
            return error;
        }
    }
    return 0;
}

int process_input_state_playing(TsuPlanarGame* game) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (tsu_sdl_quit_event(&e)) {
            game->keep_running = false;
            game->state = GameStateEnded;
            return set_game_state(game, GameStateEnded);
        }
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_n) {
            //todo: next graph
            return set_game_state(game, GameStateEnded);
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

int set_game_state(TsuPlanarGame* g, GameState state) {
    g->state = state;
    g->mouse.is_up = true;
    switch(state) {
        case GameStatePlaying:{
            int error = nodes_append_rand(g, 3);
            if(error) { return error; }
            error = tsu_draw_points(g);
            if(error) { return error; }
            g->methods.process_input = &process_input_state_playing;
            g->methods.update = &update_state_playing;
                              }
            break;
        case GameStateTitle:
            g->methods.process_input = &process_input_state_title;
            g->methods.update = &update_state_title;
            break;
            //todo: complete
        default:
            break;
    }


    return 0;
}


int update(TsuPlanarGame* game) {
    return game->methods.update(game);
}

int process_input(TsuPlanarGame* game) {
    return game->methods.process_input(game);
}
