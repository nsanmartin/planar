SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)
CFLAGS = -Wextra -Werror -Wall  -pedantic -I./include 

CC := gcc
#CFLAGS := -ggdb3 -Wall -lm -Werror `sdl2-config --libs --cflags`
BUILD := ./build
HDRS := -I./include
SRCS=./src/*.c

planar:build
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -o $(BUILD)/$@ main.c $(SRCS) $(SDL_LDFLAGS)

# planar:build
# 	$(CC) -o $(BUILD)/$@ main.c $(CFLAGS) $(SRCS) $(HDRS) 

debug:build
	$(CC) -ggdb3 $(CFLAGS) $(SDL_CFLAGS) -o $(BUILD)/$@ main.c $(SRCS) $(SDL_LDFLAGS)


build:
	if [ ! -d build ]; then mkdir build; fi

clean:
	rm $(BUILD)/*

