#pragma once
#include <stdint.h>
#include <stdbool.h>
#define FONT_START_ADDRESS 0x50

typedef struct Chip8
{
    uint8_t memory[4096];
    uint8_t display[32][64];
    uint16_t pc;
    uint16_t index_reg;
    uint16_t stack[16];
    int8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t v_registers[16]; // From V0 to VF.
    uint8_t keys[16];
    bool pressFlag;
} Chip8;

void chip8Init(Chip8* chip8);
void loadROM(Chip8* c, char* romName);
void pushStack(Chip8* chip8, uint16_t entry);
uint16_t popStack(Chip8* chip8);
