#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "SDL.h"

typedef struct TsuPlanarGame TsuPlanarGame;
TsuPlanarGame* newPlanarGameWith(size_t w, size_t h);
void freePlanarGame(TsuPlanarGame* game);
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


int main() {
    TsuPlanarGame* game = newPlanarGameWith(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (game == NULL) {
        fprintf(stderr, "Could not load game, exiting.\n");
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

        freePlanarGame(game);
    }

    return 0;
}
