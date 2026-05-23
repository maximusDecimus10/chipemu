#ifndef CPU_H
#define CPU_H
#include<stdio.h>
#include<stdint.h>


typedef struct {
    uint8_t  mem[4096]; // memory 
    uint8_t  V[16]; // 16 bit registers VF is the Flag
    uint16_t I;
    uint16_t PC;
    uint8_t  SP;
    uint16_t stack[16];
    uint8_t  delay_timer;
    uint8_t  sound_timer;
    uint8_t  display[64 * 32];
    uint8_t  keypad[16];
    uint8_t  draw_flag;
    uint8_t  running;
} Chip8;

// 1st nibble --> what ins it supposed to be
// 2nd --> usually a register number
// 3rd --> another regiter nuber (NN)
// 4th --> a small number (NNN)
//
//
//init and loadssss
void chip8_init(Chip8 *c);
void chip8_load_rom(Chip8 *c, const char *path);

void chip8_cycle(Chip8 *c);


uint16_t chip8_fetch(Chip8 *c);
void     chip8_decode_execute(Chip8 *c, uint16_t opcode);
void     chip8_update_timers(Chip8 *c);


void chip8_op_00E0(Chip8 *c);                          // clear screen
void chip8_op_00EE(Chip8 *c);                          // return
void chip8_op_1NNN(Chip8 *c, uint16_t NNN);            // jump
void chip8_op_2NNN(Chip8 *c, uint16_t NNN);            // call
void chip8_op_3XNN(Chip8 *c, uint8_t X, uint8_t NN);  // skip ==
void chip8_op_4XNN(Chip8 *c, uint8_t X, uint8_t NN);  // skip !=
void chip8_op_5XY0(Chip8 *c, uint8_t X, uint8_t Y);   // skip VX==VY
void chip8_op_6XNN(Chip8 *c, uint8_t X, uint8_t NN);  // load imm
void chip8_op_7XNN(Chip8 *c, uint8_t X, uint8_t NN);  // add imm
void chip8_op_8XY0(Chip8 *c, uint8_t X, uint8_t Y);   // VX = VY
void chip8_op_8XY1(Chip8 *c, uint8_t X, uint8_t Y);   // OR
void chip8_op_8XY2(Chip8 *c, uint8_t X, uint8_t Y);   // AND
void chip8_op_8XY3(Chip8 *c, uint8_t X, uint8_t Y);   // XOR
void chip8_op_8XY4(Chip8 *c, uint8_t X, uint8_t Y);   // add carry
void chip8_op_8XY5(Chip8 *c, uint8_t X, uint8_t Y);   // sub borrow
void chip8_op_8XY6(Chip8 *c, uint8_t X);              // shift right
void chip8_op_8XY7(Chip8 *c, uint8_t X, uint8_t Y);   // VY-VX
void chip8_op_8XYE(Chip8 *c, uint8_t X);              // shift left
void chip8_op_9XY0(Chip8 *c, uint8_t X, uint8_t Y);   // skip VX!=VY
void chip8_op_ANNN(Chip8 *c, uint16_t NNN);            // set I
void chip8_op_BNNN(Chip8 *c, uint16_t NNN);            // jump+V0
void chip8_op_CXNN(Chip8 *c, uint8_t X, uint8_t NN);  // random
void chip8_op_DXYN(Chip8 *c, uint8_t X, uint8_t Y, uint8_t N); // draw
void chip8_op_EX9E(Chip8 *c, uint8_t X);              // skip key
void chip8_op_EXA1(Chip8 *c, uint8_t X);              // skip no key
void chip8_op_FX07(Chip8 *c, uint8_t X);              // VX = DT
void chip8_op_FX0A(Chip8 *c, uint8_t X);              // wait key
void chip8_op_FX15(Chip8 *c, uint8_t X);              // DT = VX
void chip8_op_FX18(Chip8 *c, uint8_t X);              // ST = VX
void chip8_op_FX1E(Chip8 *c, uint8_t X);              // I += VX
void chip8_op_FX29(Chip8 *c, uint8_t X);              // I = font
void chip8_op_FX33(Chip8 *c, uint8_t X);              // BCD
void chip8_op_FX55(Chip8 *c, uint8_t X);              // store regs
void chip8_op_FX65(Chip8 *c, uint8_t X);              // load regs
#endif