# CHIP-8Emu
A CHIP-8 Emulator Written in C

I have written this CHIP-8 emulator to see how a processor's specifications are translated into actual code. This is my first time writing an emulator.


## CHIP-8
CHIP-8 is an interpreted programming language, developed by Joseph Weisbecker on his 1802 microprocessor. It was initially used on the COSMAC VIP and Telmac 1800, which were 8-bit microcomputers made in the mid-1970s. It is a bytecode language, allowing games to be written once and run on any machine with a CHIP-8 interpreter. This made it possible for programmers to write games without needing to know assembly language of a specific chip.


## Technical Overview
### Specifications
4096 bytes of RAM,  
16-bit program counter,  
16-bit index register,  
16 8-bit variable registers (From `v0` to `vF`),  
a stack of size 16 for 16-bit addresses,  
a 8-bit delay timer,  
a 8-bit sound timer,  
64x32 pixels monochrome display (64 pixels wide, 32 pixels tall).  

CHIP-8 has **35 opcodes**; I have implemented all of them **except 0NNN**.  

The emulator runs **≈1000 fetch-decode-execute cycles per second**, ≈1ms per cycle.  

Delay and sound timers are decremented by one every ≈16ms if their values are greater than 0. 

Sound timer makes the emulator make a beep sound using SDL2 if its value is greater than 0.

The display is updated every ≈16ms. The emulator uses SDL2 to render the display to the screen.

### Quirks
**VF Reset Quirk:** Instructions `8XY1`, `8XY2` and `8XY3` set  `vF register` to 0 after performing their logical operations.

**Load Store (Memory) Quirk:** Instructions `FX55` and `FX65` increment the index register by default. You can prevent this behavior by setting `uint8_t conf3 = 1;` line to `uint8_t conf3 = 0;`. You can find this line at the start of the main function. (e.g. Astro Dodge game needs `conf3` set to `0`.)

**Display Wait (Vertical Blank) Quirk:** Instruction `DXYN` must wait for vertical blank and allow one draw operation per frame. As far as I understand, with modern hardware, there shouldn't be sprite tearing even if we don't wait for vBlank since we fully fill the CHIP-8's display, create a texture from this array and then send this texture to SDL to render it on the screen. So, this instruction is implemented this way to match the emulation speed to the original hardware.

**Clipping Quirk:** Instruction `DXYN` clips the sprite instead of wrapping around. You can change this behavior easily by modifying this instruction's implementation in the code.

**Jumping Quirk:** Instruction `BNNN` jumps to the address "value of `v0 register` + NNN" by default. You can change this behavior to make this instruction jump to "value of `vX register` + NNN" by setting `uint8_t conf2 = 0;` line to `uint8_t conf2 = 1;`. You can find this line at the start of the main function.

**Shift Quirk:** Instructions `8XY6` and `8XYE` ignore `vY register` and shift `vX register` by default. However, some games require loading `vY register` to `vX register` before making shifting a operation on `vX register`. You can use `-s` parameter to run games that needs this behavior. (e.g. Space Invaders needs this modification to run properly.)
e.g. `$ ./chip8emu -s <ROM path>` 


## Screenshots
### Rush Hour
![CHIP-8 Rush Hour](https://raw.githubusercontent.com/akarcorabatu/CHIP-8Emu/refs/heads/main/screenshots/rush_hour1.png)

![CHIP-8 Rush Hour](https://raw.githubusercontent.com/akarcorabatu/CHIP-8Emu/refs/heads/main/screenshots/rush_hour2.png)

### Astro Dodge
![CHIP-8 Astro Dodge](https://raw.githubusercontent.com/akarcorabatu/CHIP-8Emu/refs/heads/main/screenshots/astrododge1.png)

![CHIP-8 Astro Dodge](https://raw.githubusercontent.com/akarcorabatu/CHIP-8Emu/refs/heads/main/screenshots/astrododge2.png)

### Space Invaders
![CHIP-8 Space Invaders](https://raw.githubusercontent.com/akarcorabatu/CHIP-8Emu/refs/heads/main/screenshots/space_invaders1.png)

![CHIP-8 Space Invaders](https://raw.githubusercontent.com/akarcorabatu/CHIP-8Emu/refs/heads/main/screenshots/space_invaders2.png)


## Compiling
You need **SDL2** to compile the code.

### Linux
```
$ sudo apt install build-essential cmake libsdl2-dev
$ mkdir build
$ cd build
$ cmake ..
$ make
```


## Running
### Linux
**Default**
```
$ ./chip8emu <ROM path>
```

**With Other Shifting Configuration**
```
$ ./chip8emu -s <ROM path>
```

## Keyboard Controls
**Original CHIP-8 Keypad Layout**
<table>
<tr><td>1</td><td>2</td><td>3</td><td>C</td></tr>
<tr><td>4</td><td>5</td><td>6</td><td>D</td></tr>
<tr><td>7</td><td>8</td><td>9</td><td>E</td></tr>
<tr><td>A</td><td>0</td><td>B</td><td>F</td></tr>
</table>

**Emulator Keypad Layout**
<table>
<tr><td>1</td><td>2</td><td>3</td><td>4</td></tr>
<tr><td>Q</td><td>W</td><td>E</td><td>R</td></tr>
<tr><td>A</td><td>S</td><td>D</td><td>F</td></tr>
<tr><td>Z</td><td>X</td><td>C</td><td>V</td></tr>
</table>

You can exit the emulator pressing "L" key on the keyboard.

## License
This project is licensed under the MIT License. See the LICENSE file for full license text. 

© 2025 Akar Çorabatu
