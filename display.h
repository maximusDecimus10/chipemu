#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include "cpu.h"

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 320
#define SCALE         10

typedef struct {

    SDL_Window   *window;
    SDL_Renderer *renderer;

} Display;

int  display_init(Display *d);

void display_render(Display *d, Chip8 *c);

void display_destroy(Display *d);

#endif