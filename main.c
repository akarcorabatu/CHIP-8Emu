#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "chip8.h"
#include <time.h>
#include <math.h>


// SDL Beep sound configurations and callback function.
#define SAMPLE_RATE 44100
#define TONE_FREQ 440
#define AMPLITUDE 28000

typedef struct AudioData 
{
    uint8_t sound_timer;
    double phase;
} AudioData;

void audio_callback(void* userdata, Uint8* stream, int len) 
{
    AudioData* data = (AudioData*)userdata;
    Sint16* buffer = (Sint16*)stream;
    int samples = len / 2;

    for(int i = 0; i < samples; i++) 
    {
        if (data->sound_timer > 0) 
        {
            buffer[i] = (Sint16)(AMPLITUDE * sin(2.0 * 3.1415 * TONE_FREQ * data->phase / SAMPLE_RATE));
            data->phase += 1.0;
        } 

        else 
        {
            buffer[i] = 0;
        }
    }
}

// Decodes and executes the OpCode.
// conf1 == 0 if you want to do c.v_registers[x] = c.v_registers[y] operation before shifting.
// conf2 == 0 if you want to jump to V0 + NNN, conf2 == 1 if you want to jump to VX + NNN
// conf3 == 0 if you do not want to increment the index register when doing store and load operations.
void decodeExec(uint16_t opcode, Chip8* c, uint8_t conf1, uint8_t conf2, uint8_t conf3)
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t n = opcode & 0x000F;
    uint8_t nn = opcode & 0x00FF;
    uint16_t nnn = opcode & 0x0FFF;

    switch (opcode & 0xF000)
    {
        case 0x0000:
            switch(opcode & 0x00FF)
            {
                case 0x00E0:
                    for (int i = 0; i < 32; i++)
                    {
                        for (int j = 0; j < 64; j++)
                        {
                            c->display[i][j] = 0;
                        }
                    }            
                    c->pc += 2;
                    break;
                case 0x00EE:
                    c->pc = popStack(c);
                    break;
            }
            break;
        
        case 0x1000:
            c->pc = nnn;
            break;

        case 0x2000:
            c->pc += 2;
            pushStack(c, c->pc);
            c->pc = nnn;
            break;

        case 0x3000:
            if(c->v_registers[x] == nn) {c->pc+=4;}
            else{c->pc+=2;}
            break;

        case 0x4000:
            if(c->v_registers[x] != nn) {c->pc+=4;}
            else{c->pc+=2;}
            break;
            
        case 0x5000:
            if(c->v_registers[x] == c->v_registers[y]) {c->pc+=4;}
            else{c->pc+=2;}
            break;

        case 0x6000:
            c->v_registers[x] = nn;
            c->pc+=2;
            break;

        case 0x7000:
            c->v_registers[x]+= nn;
            c->pc+=2;
            break;
        
        case 0x8000:
            switch(opcode & 0x000F)
            {
                case 0x0000:
                    c->v_registers[x] = c->v_registers[y];
                    c->pc+=2;
                    break;

                case 0x0001:
                    c->v_registers[x] = c->v_registers[x] | c->v_registers[y];
                    c->v_registers[0xF] = 0;
                    c->pc+=2;
                    break;

                case 0x0002:
                    c->v_registers[x] = c->v_registers[x] & c->v_registers[y];
                    c->v_registers[0xF] = 0;
                    c->pc+=2;
                    break;

                case 0x0003:
                    c->v_registers[x] = c->v_registers[x] ^ c->v_registers[y];
                    c->v_registers[0xF] = 0;
                    c->pc+=2;
                    break;

                case 0x0004:
                    uint16_t sum = (uint16_t)c->v_registers[x] + (uint16_t)c->v_registers[y];
                    c->v_registers[x] = (uint8_t)sum;
                    c->v_registers[0xF] = (sum > 0xFF) ? 1 : 0;
                    c->pc+=2;
                    break;

                case 0x0005:
                    uint8_t subtrahend = c->v_registers[x];
                    c->v_registers[x] = c->v_registers[x] - c->v_registers[y];
                    if(subtrahend >= c->v_registers[y]){c->v_registers[0xF] = 1;}
                    else{c->v_registers[0xF] = 0;}
                    c->pc+=2;
                    break;

                case 0x0006:
                    if(conf1 == 0){c->v_registers[x] = c->v_registers[y];}
                    uint8_t lsb = c->v_registers[x] & 0x01;
                    c->v_registers[x] = c->v_registers[x] >> 1;
                    c->v_registers[0xF] = lsb ? 1 : 0;
                    c->pc+=2;
                    break;

                case 0x0007:
                    c->v_registers[x] = c->v_registers[y] - c->v_registers[x];
                    if(c->v_registers[y] >= c->v_registers[x]){c->v_registers[0xF] = 1;}
                    else{c->v_registers[0xF] = 0;}
                    c->pc+=2;
                    break;
                
                case 0x000E:
                    if(conf1 == 0){c->v_registers[x] = c->v_registers[y];}
                    uint8_t msb = c->v_registers[x] & 0x80;
                    c->v_registers[x] = c->v_registers[x] << 1;
                    c->v_registers[0xF] = msb ? 1 : 0;
                    c->pc+=2;
                    break;
            }
            break;

        case 0x9000:
            if(c->v_registers[x] != c->v_registers[y]) {c->pc+=4;}
            else{c->pc+=2;}
            break;
           
        case 0xA000:
            c->index_reg = nnn;
            c->pc += 2;
            break;
        
        case 0xB000:
            if(conf2 == 0) 
            {
                c->pc = nnn + c->v_registers[0x0];
            }
            else
            {
                c->pc = nnn + c->v_registers[x];
            }
            break;
        
        case 0xC000:
            c->v_registers[x] = (uint8_t)((rand() % 256) & nn);
            c->pc+=2;
            break;

        case 0xD000:
            uint8_t xPos = c->v_registers[x] % 64;
            uint8_t yPos = c->v_registers[y] % 32;
            c->v_registers[0xF] = 0;
            for (int i = 0; i < n; i++)
            {
                uint8_t spriteData = c->memory[c->index_reg + i];
                for (int j = 0; j < 8; j++)
                {
                    uint8_t current = (spriteData & (0x80 >> j)) ? 1 : 0;
                    if(current == 1)
                    {
                        int xpos = (xPos + j);
                        if(xpos >= 64){break;}
                        int ypos = (yPos + i);
                        if(ypos >= 32){break;}  

                        if(c->display[ypos][xpos] == 1) c->v_registers[0xF] = 1;

                        c->display[ypos][xpos] ^= 1;
                        
                    }
                    
                }
            }
            c->pc += 2;
            break;
        case 0xE000:
            switch (opcode & 0x00FF)
            {
            case 0x009E:
                if(c->keys[c->v_registers[x]] == 1){c->pc+=2;}
                c->pc+=2;
                break;
            
            case 0x00A1:
                if(c->keys[c->v_registers[x]] == 0){c->pc+=2;}
                c->pc+=2;
                break;
            }
            break;

        case 0xF000:
            switch (opcode & 0x00FF)
            {
            case 0x0007:
                c->v_registers[x] = c->delay_timer;
                c->pc+=2;
                break;

            case 0x000A:
            uint8_t keyPressed;
                for (uint8_t i = 0; i < 16; i++)
                {
                    if(c->keys[i] == 1) 
                    {
                        c->v_registers[x] = i; 
                        keyPressed = i;
                        c->pressFlag = 1;
                    }
                }

                if(c->keys[keyPressed] == 0 && c->pressFlag == 1) 
                {
                    c->pc+=2; 
                    c->pressFlag = 0;
                }
                break;
            
            case 0x0015:
                c->delay_timer = c->v_registers[x];
                c->pc+=2;
                break;

            case 0x0018:
                c->sound_timer = c->v_registers[x];
                c->pc+=2;
                break;
            
            case 0x001E:
                c->index_reg+= c->v_registers[x];
                if(c->index_reg >= 0x1000){c->v_registers[0xF] = 1;}
                else{c->v_registers[0xF] = 0;}
                c->pc+=2;
                break;
            
            case 0x0029:
                c->index_reg = FONT_START_ADDRESS + (c->v_registers[x] * 5);
                c->pc+=2;
                break;

            case 0x0033:
                c->memory[c->index_reg] = c->v_registers[x] / 100;
                c->memory[c->index_reg + 1] = (c->v_registers[x] / 10) % 10;
                c->memory[c->index_reg + 2] = c->v_registers[x] % 10;
                c->pc+=2;
                break;
            
            case 0x0055:
                for (uint8_t i = 0; i <= x; i++)
                {
                    if(conf3 == 0){c->memory[c->index_reg + i] = c->v_registers[i];}
                    else{c->memory[c->index_reg] = c->v_registers[i]; c->index_reg++;}
                }
                c->pc+=2;
                break;

            case 0x0065:
                for (uint8_t i = 0; i <= x; i++)
                {
                    if(conf3 == 0){c->v_registers[i] = c->memory[c->index_reg + i];}
                    else{c->v_registers[i] = c->memory[c->index_reg]; c->index_reg++;}
                }
                c->pc+=2;
                break;
            }
            break;
    }
}

// Draws the chip8.display[32][64] on the screen using SDL.
void drawDisplay(Chip8* c, SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture)
{
    uint32_t pixels[64 * 32];

    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            pixels[y * 64 + x] = c->display[y][x] ? 0x42B0F5FF : 0x000000FF;
        }
    }

    SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    // Arguments to run the emulator with a selected file.
    if(argc != 2)
    {
        if(argc > 2)
        {
            printf("Please only enter ROM name.\n");
            return 0;
        }

        else {printf("Please enter ROM name. (Example: ./main test.ch8)\n"); return 0;}
    }

    char* romName = argv[1];

    srand(time(NULL));

    bool running = true;
    // Creating the window with SDL.
    SDL_Event event;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    
    int windowScale = 24;
    int windowWidth = 64 * windowScale;
    int windowHeight = 32 * windowScale;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
    {
        printf("SDL error: %s\n", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow
    (
        "CHIP-8_Emu",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_SHOWN
    );

    if(window == NULL)
    {
        printf("SDL Window error: %s\n", SDL_GetError());
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        64,32);

    // SDL Beep sound configuration.
    SDL_AudioSpec want, have;
    AudioData audio_data = {0, 0};

    SDL_zero(want);
    want.freq = SAMPLE_RATE;
    want.format = AUDIO_S16SYS;
    want.channels = 1;
    want.samples = 2048;
    want.callback = audio_callback;
    want.userdata = &audio_data;

    SDL_OpenAudio(&want, &have);
    SDL_PauseAudio(0);

    Chip8 chip8;
    chip8Init(&chip8);
    loadROM(&chip8, romName);

    int keymap[16] = 
    {
        SDLK_x,    // 0
        SDLK_1,    // 1
        SDLK_2,    // 2
        SDLK_3,    // 3
        SDLK_q,    // 4
        SDLK_w,    // 5
        SDLK_e,    // 6
        SDLK_a,    // 7
        SDLK_s,    // 8
        SDLK_d,    // 9
        SDLK_z,    // A
        SDLK_c,    // B
        SDLK_4,    // C
        SDLK_r,    // D
        SDLK_f,    // E
        SDLK_v     // F
    };

    uint32_t msPerOp = 1;

    uint32_t msPerDecrement = 16;
    uint32_t decrementTimer = SDL_GetTicks(); 

    while(running) 
    { 
        uint32_t start = SDL_GetTicks(); 

        while(SDL_PollEvent(&event)) 
        { 
            if(event.type == SDL_QUIT) {running = false;} 
            else if(event.type == SDL_KEYDOWN){if(event.key.keysym.sym == SDLK_l) {running = false;}} 

            // Input
            if (event.type == SDL_KEYDOWN) 
            { 
                for (int i = 0; i < 16; i++) 
                { 
                    if (event.key.keysym.sym == keymap[i]){chip8.keys[i] = 1;} 
                } 
            } 

            if (event.type == SDL_KEYUP) 
            { 
                for (int i = 0; i < 16; i++) 
                { 
                    if (event.key.keysym.sym == keymap[i]){chip8.keys[i] = 0;} 
                } 
            } 
        } 
        
        uint16_t opcode = (chip8.memory[chip8.pc] << 8) | chip8.memory[chip8.pc + 1]; // Fetch instruction from memory. 
        decodeExec(opcode, &chip8, 0, 0, 1); // Decode and execute.

        if (chip8.sound_timer > 0) {audio_data.sound_timer = chip8.sound_timer;} // Beep if sound timer is greater than > 0.
        else {audio_data.sound_timer = 0;} 
        
        if(SDL_GetTicks() - decrementTimer >= msPerDecrement)
        { 
            if(chip8.delay_timer > 0) chip8.delay_timer--; 
            if(chip8.sound_timer > 0) chip8.sound_timer--; 
            decrementTimer = SDL_GetTicks(); 
            drawDisplay(&chip8, window, renderer, texture);
        } 
        
        uint32_t elapsed = SDL_GetTicks() - start; 
        if(elapsed < msPerOp){SDL_Delay(msPerOp - elapsed);} 
    }
    return 0;
}
