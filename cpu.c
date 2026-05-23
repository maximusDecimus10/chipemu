#include "cpu.h"
#include <string.h>
#include<stdio.h>
#include <stdlib.h>

void chip8_init(Chip8 *c) {

    memset(c, 0, sizeof(Chip8));

    c->PC      = 0x200;
    c->running = 1;

    uint8_t fonts[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
        0x20, 0x60, 0x20, 0x20, 0x70,  // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
        0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
        0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
        0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
        0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
        0xF0, 0x80, 0xF0, 0x80, 0x80,  // F
    };

    memcpy(c->mem, fonts, sizeof(fonts));
}

void chip8_load_rom(Chip8 *c, const char *path) {

    FILE *rom = fopen(path, "rb");

    if (rom == NULL) {
        printf("error: could not open rom file %s\n", path);
        c->running = 0;
        return;
    }

    fread(&c->mem[0x200], 1, sizeof(c->mem), rom);

    fclose(rom);
}


uint16_t chip8_fetch(Chip8 *c){
    uint16_t opcode = (c->mem[c->PC] << 8) | c->mem[c->PC + 1];
    c->PC += 2;
    return opcode; 
}

void chip8_update_timers(Chip8 *c)
{
    if (c->delay_timer > 0)
        c->delay_timer--;

    if (c->sound_timer > 0)
        c->sound_timer--;
}

void chip8_decode_execute(Chip8 *c, uint16_t opcode)
{
    uint8_t  X   = (opcode >> 8) & 0x0F;
    uint8_t  Y   = (opcode >> 4) & 0x0F;
    uint8_t  N   =  opcode & 0x0F;
    uint8_t  NN  =  opcode & 0xFF;
    uint16_t NNN =  opcode & 0x0FFF;

    switch (opcode & 0xF000) {

        case 0x0000:
            switch (opcode) {
                case 0x00E0: chip8_op_00E0(c); break;
                case 0x00EE: chip8_op_00EE(c); break;
                default: printf("Unknown opcode: %04X\n", opcode); c->running = 0; break;
            }
            break;

        case 0x1000: chip8_op_1NNN(c, NNN); break;
        case 0x2000: chip8_op_2NNN(c, NNN); break;
        case 0x3000: chip8_op_3XNN(c, X, NN); break;
        case 0x4000: chip8_op_4XNN(c, X, NN); break;

        case 0x5000:
            if (N == 0) chip8_op_5XY0(c, X, Y);
            else { printf("Unknown opcode: %04X\n", opcode); c->running = 0; }
            break;

        case 0x6000: chip8_op_6XNN(c, X, NN); break;
        case 0x7000: chip8_op_7XNN(c, X, NN); break;

        case 0x8000:
            switch (N) {
                case 0x0: chip8_op_8XY0(c, X, Y); break;
                case 0x1: chip8_op_8XY1(c, X, Y); break;
                case 0x2: chip8_op_8XY2(c, X, Y); break;
                case 0x3: chip8_op_8XY3(c, X, Y); break;
                case 0x4: chip8_op_8XY4(c, X, Y); break;
                case 0x5: chip8_op_8XY5(c, X, Y); break;
                case 0x6: chip8_op_8XY6(c, X); break;
                case 0x7: chip8_op_8XY7(c, X, Y); break;
                case 0xE: chip8_op_8XYE(c, X); break;
                default: printf("Unknown opcode: %04X\n", opcode); c->running = 0; break;
            }
            break;

        case 0x9000:
            if (N == 0) chip8_op_9XY0(c, X, Y);
            else { printf("Unknown opcode: %04X\n", opcode); c->running = 0; }
            break;

        case 0xA000: chip8_op_ANNN(c, NNN); break;
        case 0xB000: chip8_op_BNNN(c, NNN); break;
        case 0xC000: chip8_op_CXNN(c, X, NN); break;
        case 0xD000: chip8_op_DXYN(c, X, Y, N); break;

        case 0xE000:
            switch (NN) {
                case 0x9E: chip8_op_EX9E(c, X); break;
                case 0xA1: chip8_op_EXA1(c, X); break;
                default: printf("Unknown opcode: %04X\n", opcode); c->running = 0; break;
            }
            break;

        case 0xF000:
            switch (NN) {
                case 0x07: chip8_op_FX07(c, X); break;
                case 0x0A: chip8_op_FX0A(c, X); break;
                case 0x15: chip8_op_FX15(c, X); break;
                case 0x18: chip8_op_FX18(c, X); break;
                case 0x1E: chip8_op_FX1E(c, X); break;
                case 0x29: chip8_op_FX29(c, X); break;
                case 0x33: chip8_op_FX33(c, X); break;
                case 0x55: chip8_op_FX55(c, X); break;
                case 0x65: chip8_op_FX65(c, X); break;
                default: printf("Unknown opcode: %04X\n", opcode); c->running = 0; break;
            }
            break;

        default:
            printf("Unknown opcode: %04X\n", opcode);
            c->running = 0;
            break;
    }
}
void chip8_op_00E0(Chip8 *c){
    memset(c->display, 0, sizeof(c->display));
    c->draw_flag = 1;
}
void chip8_op_00EE(Chip8 *c){
    c->SP--;
    c->PC = c->stack[c->SP];
}
void chip8_op_1NNN(Chip8 *c, uint16_t NNN){
    c->PC = NNN;
}

void chip8_op_2NNN(Chip8* c, uint16_t NNN){
    c->stack[c->SP] = c->PC;
    c->SP++;
    c->PC = NNN;
}
void chip8_op_3XNN(Chip8 *c, uint8_t X, uint8_t NN) {
    if (c->V[X] == NN)
        c->PC += 2;
}

void chip8_op_4XNN(Chip8 *c, uint8_t X, uint8_t NN){
    if(c->V[X] != NN) 
        c->PC += 2;
}  // skip !=
void chip8_op_5XY0(Chip8 *c, uint8_t X, uint8_t Y){
    if(c->V[X] == c->V[Y])
      c->PC += 2;
}   // skip VX==VY
void chip8_op_6XNN(Chip8 *c, uint8_t X, uint8_t NN){
    c->V[X] = NN;
}  // load imm
void chip8_op_7XNN(Chip8 *c, uint8_t X, uint8_t NN){
    c->V[X] = c->V[X] + NN;
}  // add imm
void chip8_op_8XY0(Chip8 *c, uint8_t X, uint8_t Y){
    c->V[X] = c->V[Y];
}   // VX = VY
void chip8_op_8XY1(Chip8 *c, uint8_t X, uint8_t Y){
    c->V[X] = c->V[X] | c->V[Y];
}   // OR
void chip8_op_8XY2(Chip8 *c, uint8_t X, uint8_t Y){
    c->V[X] = c->V[X] & c->V[Y];
}   // AND
void chip8_op_8XY3(Chip8 *c, uint8_t X, uint8_t Y){
    c->V[X] = c->V[X] ^ c->V[Y];
}  // XOR
void chip8_op_8XY4(Chip8 *c, uint8_t X, uint8_t Y){
    uint16_t tmp = c->V[X]+c->V[Y];
    c->V[X] = (tmp & 0x00FF);
    c->V[15] =  (tmp>>8) & 0x00FF;
}  // add carry
void chip8_op_8XY5(Chip8 *c, uint8_t X, uint8_t Y){
    c->V[0xF] = (c->V[X] >= c->V[Y]);
    c->V[X] -= c->V[Y];
}  // sub borrow
void chip8_op_8XY6(Chip8 *c, uint8_t X){
    c->V[15] = c->V[X] & 0x01;
    c->V[X] = c->V[X] >> 1;
}              // shift right
void chip8_op_8XY7(Chip8 *c, uint8_t X, uint8_t Y){
    c->V[0xF] = (c->V[Y] >= c->V[X]);
    c->V[X] = c->V[Y] - c->V[X];
}  // VY-VX
void chip8_op_8XYE(Chip8 *c, uint8_t X){
    c->V[0xF] = (c->V[X] & 0x80)>>7;
    c->V[X] = c->V[X] << 1;
}           // shift left
void chip8_op_9XY0(Chip8 *c, uint8_t X, uint8_t Y){
    if (c->V[X] != c->V[Y])
        c->PC += 2;
}  // skip VX!=VY
void chip8_op_ANNN(Chip8 *c, uint16_t NNN){
    c->I = NNN;
}           // set I
void chip8_op_BNNN(Chip8 *c, uint16_t NNN)
{
    c->PC = NNN + c->V[0];
}           // jump+V0


void chip8_op_CXNN(Chip8 *c, uint8_t X, uint8_t NN)
{
    c->V[X] = (rand() % 256) & NN;
}// random
void chip8_op_DXYN(Chip8 *c, uint8_t X, uint8_t Y, uint8_t N){
    c->V[0xF] = 0x0;
    for(uint8_t i = 0; i < N; i++){
        uint8_t sprite_byte = c->mem[c->I + i];
        for(uint8_t j = 0; j < 8; j++){
            uint8_t bit = (sprite_byte >> (7 - j)) & 1;
            if(bit){
                uint8_t x = (c->V[X] + j) % 64;
                uint8_t y = (c->V[Y] + i) % 32;
                uint16_t ind = x + y * 64;
                if(c->display[ind] == 1)
                    c->V[0xF] = 1;
                c->display[ind] ^= 1;
            }
        }
    }
    c->draw_flag = 1;
}// draw
void chip8_op_EX9E(Chip8 *c, uint8_t X){
    if(c->keypad[c->V[X]] == 1)
        c->PC+=2;
}              // skip key
void chip8_op_EXA1(Chip8 *c, uint8_t X){
    if(c->keypad[c->V[X]] == 0) c->PC+=2;
}             // skip no key
void chip8_op_FX07(Chip8 *c, uint8_t X){
    c->V[X] = c->delay_timer;
}              // VX = DT

void chip8_op_FX0A(Chip8 *c, uint8_t X){

    for(uint8_t i = 0; i < 16; i++){

        if(c->keypad[i]){

            c->V[X] = i;
            return;
        }
    }

    c->PC -= 2;
}              // wait key

void chip8_op_FX15(Chip8 *c, uint8_t X){
    c->delay_timer = c->V[X];
}              // DT = VX

void chip8_op_FX18(Chip8 *c, uint8_t X){
    c->sound_timer = c->V[X];
}              // ST = VX

void chip8_op_FX1E(Chip8 *c, uint8_t X){
    c->I += c->V[X];
}              // I += VX

void chip8_op_FX29(Chip8 *c, uint8_t X){
    c->I = c->V[X] * 5;
}              // I = font

void chip8_op_FX33(Chip8 *c, uint8_t X){

    uint8_t val = c->V[X];

    c->mem[c->I]     = val / 100;
    c->mem[c->I + 1] = (val / 10) % 10;
    c->mem[c->I + 2] = val % 10;
}              // BCD

void chip8_op_FX55(Chip8 *c, uint8_t X){

    for(uint8_t i = 0; i <= X; i++)
        c->mem[c->I + i] = c->V[i];
}              // store regs

void chip8_op_FX65(Chip8 *c, uint8_t X){

    for(uint8_t i = 0; i <= X; i++)
        c->V[i] = c->mem[c->I + i];
}              // load regs