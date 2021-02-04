#pragma once

#include <iostream>
#include <cstdint>
#include <stack>
#include <fstream>
#include <SDL.h>

class Chippy
{
public:
	Chippy();

	void load(char* buffer, std::streampos size);
	void opcode();

	// Destructor to be implemented at some point
	~Chippy();

private:
	/*Memory Map:
	+---------------+= 0xFFF (4095) End of Chip-8 RAM
	|               |
	|               |
	|               |
	|               |
	|               |
	| 0x200 to 0xFFF|
	|     Chip-8    |
	| Program / Data|
	|     Space     |
	|               |
	|               |
	|               |
	+- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
	|               |
	|               |
	|               |
	+---------------+= 0x200 (512) Start of most Chip-8 programs
	| 0x000 to 0x1FF|
	| Reserved for  |
	|  interpreter  |
	+---------------+= 0x000 (0) Start of Chip-8 RAM*/
	uint8_t MEMORY[4096];
	int PROGRAM_START = 0x200;

	// I'm not sure where exactly it should be started other than that
	// it should be between 0x000-0x1FF, so lets just put it near the beginning.
	// Also there are 16 sprites, each 5 bytes in size
	// Reference (Display): http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
	int SPRITESET_START = 0x10;
	uint8_t sprites[16 * 5] =
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

	/*Registers*/
	uint8_t V[16]; // 16 general 8-bit registers V0-VF
	uint16_t I; // 16-bit register to store memory addresses, lowest 12 bits are used

	/*Timers, rate of 60hz*/
	uint8_t DELAY_TIMER;
	uint8_t SOUND_TIMER;

	/*pseudo-regsssss*/
	uint16_t PC; // program counter

	/*Big fat stack*/
	std::stack<uint16_t> STACK;

	/*High res display*/
	uint8_t DISPLAY[64 * 32];
};