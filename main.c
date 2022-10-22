#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "SDL.h"
#include "TsuPlanarGame.h"

int update(TsuPlanarGame* game);
void render(TsuPlanarGame* game);
int process_input(TsuPlanarGame* game);
bool keep_running(const TsuPlanarGame* game);

#define WINDOW_WIDTH 640UL
#define WINDOW_HEIGHT 480UL


enum { NS_PER_SECOND = 1000000000, MS_PER_SECOND = 1000, NS_PER_MS = 1000000 };
long get_time_millis() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec * MS_PER_SECOND + t.tv_nsec / NS_PER_MS;
}


int run_game(TsuPlanarGame* game) {
    int error = init_planar_game(game, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (error) {
        fprintf(stderr, "Could not load game, exiting.\n");
        return error;
    } else {
        long previous = get_time_millis();
        long lag = 0;

        while (keep_running(game)) {
            long current = get_time_millis();
            long elapsed = current - previous;
            previous = current;
            lag += elapsed;

            process_input(game);
            update(game);
            render(game);
        }

        destroy_planar_game(game);
    }
    return 0;
}

int main() {
    TsuPlanarGame game;
    run_game(&game);

    return 0;
}
