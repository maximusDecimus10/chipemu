// main.c

#include <stdio.h>
#include <SDL2/SDL.h>

#include "cpu.h"
#include "display.h"

int main(int argc, char *argv[]) {

    if(argc != 2){

        printf("usage: ./chip8 <rom>\n");

        return 1;
    }

    Chip8 c;

    Display d;

    chip8_init(&c);

    if(!display_init(&d))
        return 1;

    chip8_load_rom(&c, argv[1]);

    SDL_Event event;

    while(c.running){

        while(SDL_PollEvent(&event)){

            if(event.type == SDL_QUIT)
                c.running = 0;

            if(event.type == SDL_KEYDOWN){

                switch(event.key.keysym.sym){

                    case SDLK_1: c.keypad[0x1] = 1; break;
                    case SDLK_2: c.keypad[0x2] = 1; break;
                    case SDLK_3: c.keypad[0x3] = 1; break;
                    case SDLK_4: c.keypad[0xC] = 1; break;

                    case SDLK_q: c.keypad[0x4] = 1; break;
                    case SDLK_w: c.keypad[0x5] = 1; break;
                    case SDLK_e: c.keypad[0x6] = 1; break;
                    case SDLK_r: c.keypad[0xD] = 1; break;

                    case SDLK_a: c.keypad[0x7] = 1; break;
                    case SDLK_s: c.keypad[0x8] = 1; break;
                    case SDLK_d: c.keypad[0x9] = 1; break;
                    case SDLK_f: c.keypad[0xE] = 1; break;

                    case SDLK_z: c.keypad[0xA] = 1; break;
                    case SDLK_x: c.keypad[0x0] = 1; break;
                    case SDLK_c: c.keypad[0xB] = 1; break;
                    case SDLK_v: c.keypad[0xF] = 1; break;
                }
            }

            if(event.type == SDL_KEYUP){

                switch(event.key.keysym.sym){

                    case SDLK_1: c.keypad[0x1] = 0; break;
                    case SDLK_2: c.keypad[0x2] = 0; break;
                    case SDLK_3: c.keypad[0x3] = 0; break;
                    case SDLK_4: c.keypad[0xC] = 0; break;

                    case SDLK_q: c.keypad[0x4] = 0; break;
                    case SDLK_w: c.keypad[0x5] = 0; break;
                    case SDLK_e: c.keypad[0x6] = 0; break;
                    case SDLK_r: c.keypad[0xD] = 0; break;

                    case SDLK_a: c.keypad[0x7] = 0; break;
                    case SDLK_s: c.keypad[0x8] = 0; break;
                    case SDLK_d: c.keypad[0x9] = 0; break;
                    case SDLK_f: c.keypad[0xE] = 0; break;

                    case SDLK_z: c.keypad[0xA] = 0; break;
                    case SDLK_x: c.keypad[0x0] = 0; break;
                    case SDLK_c: c.keypad[0xB] = 0; break;
                    case SDLK_v: c.keypad[0xF] = 0; break;
                }
            }
        }

        uint16_t opcode = chip8_fetch(&c);

        chip8_decode_execute(&c, opcode);

        chip8_update_timers(&c);

        if(c.draw_flag){

            display_render(&d, &c);

            c.draw_flag = 0;
        }

        SDL_Delay(16);
    }

    display_destroy(&d);

    return 0;
}