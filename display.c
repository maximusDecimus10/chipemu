#include "display.h"
#include <stdio.h>

int display_init(Display *d){

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("SDL init failed: %s\n", SDL_GetError());
        return 0;
    }

    d->window = SDL_CreateWindow(
        "CHIP-8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if(d->window == NULL){
        printf("window creation failed: %s\n", SDL_GetError());
        return 0;
    }

    d->renderer = SDL_CreateRenderer(
        d->window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if(d->renderer == NULL){
        printf("renderer creation failed: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

void display_render(Display *d, Chip8 *c){

    SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
    SDL_RenderClear(d->renderer);

    SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255);

    for(int y = 0; y < 32; y++){

        for(int x = 0; x < 64; x++){

            uint16_t ind = x + y * 64;

            if(c->display[ind]){

                SDL_Rect rect;

                rect.x = x * SCALE;
                rect.y = y * SCALE;
                rect.w = SCALE;
                rect.h = SCALE;

                SDL_RenderFillRect(d->renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(d->renderer);

    c->draw_flag = 0;
}

void display_destroy(Display *d){

    SDL_DestroyRenderer(d->renderer);

    SDL_DestroyWindow(d->window);

    SDL_Quit();
}