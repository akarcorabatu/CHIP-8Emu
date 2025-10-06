#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void chip8Init(Chip8* chip8)
{
    memset(chip8->memory, 0, sizeof(chip8->memory));
    memset(chip8->display, 0, sizeof(chip8->display));
    memset(chip8->stack, 0, sizeof(chip8->stack));
    memset(chip8->v_registers, 0, sizeof(chip8->v_registers));
    memset(chip8->keys, 0, sizeof(chip8->keys));

    chip8->pc = 0;
    chip8->index_reg = 0;
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
    chip8->sp = -1;
    chip8->pressFlag = false;


    chip8->pc = ROM_START_ADDRESS; // ROM start address.
    uint8_t fontSet[80] = 
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for (int i = 0; i < 80; i++) // Load font set.
    {
        chip8->memory[FONT_START_ADDRESS + i] = fontSet[i];
    }
    
}

void pushStack(Chip8* chip8, uint16_t entry)
{
    if (chip8->sp < 15)
    {
        chip8->sp++;
        chip8->stack[chip8->sp] = entry;
    }
    else {printf("Stack overflow.\n");}
}

uint16_t popStack(Chip8* chip8)
{
    if(chip8->sp >= 0)
    {
        uint16_t temp = chip8->stack[chip8->sp];
        chip8->sp--;
        return temp;
    }

    else{printf("Stack underflow.\n"); return 0;}
}

void loadROM(Chip8* c, char* romName)
{
    FILE *file = fopen(romName, "rb");
    if(!file){printf("Cannot read ROM.\n"); return;}

    uint8_t byte;
    int i = 0;
    while(fread(&byte, 1, 1, file) == 1)
    {
        c->memory[ROM_START_ADDRESS + i] = byte;
        i++;
    }

    fclose(file);
}


