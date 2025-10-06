# CHIP-8Emu
A CHIP-8 Emulator Written in C

I have written this CHIP-8 emulator to see how a processor's specifications are translated into actual code. This is my first time writing an emulator.

## CHIP-8
CHIP-8 is an interpreted programming language, developed by Joseph Weisbecker on his 1802 microprocessor. It was initially used on the COSMAC VIP and Telmac 1800, which were 8-bit microcomputers made in the mid-1970s. It is a bytecode language, allowing games to be written once and run on any machine with a CHIP-8 interpreter. This made it possible for programmers to write games without needing to know assembly language of a specific chip.

## Screenshots
### Rush Hour
![CHIP-8 Rush Hour](https://raw.githubusercontent.com/akarcorabatu/CHIP-8Emu/refs/heads/main/screenshots/rush_hour_1.png)

![CHIP-8 Rush Hour](https://raw.githubusercontent.com/akarcorabatu/CHIP-8Emu/refs/heads/main/screenshots/rush_hour_2.png)

### Tetris
![CHIP-8 Tetris](https://raw.githubusercontent.com/akarcorabatu/CHIP-8Emu/refs/heads/main/screenshots/tetris.png)

## Compiling
### Linux
```
sudo apt install build-essential cmake libsdl2-dev
mkdir build
cd build
cmake ..
make
```

## Running
### Linux
```
./chip8emu <ROM path>
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


## License
This project is licensed under the MIT License. See the LICENSE file for full license text. 

© 2025 Akar Çorabatu
