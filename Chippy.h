#pragma once

#include <cstdint>
#include <stack>

class Chippy
{
public:
	Chippy();

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

	/*Registers*/
	uint8_t V[16]; // 16 general 8-bit registers V0-VF
	uint16_t I; // 16-bit register to store memory addresses, lowest 12 bits are used

	/*Timers, rate of 60hz*/
	uint8_t DELAY_TIMER;
	uint8_t SOUND_TIMER;

	/*pseudo-regsssss*/
	uint16_t PC; // program counter
	//uint8_t SP; // stack pointer <-- might not need this

	/*Big fat stack*/
	std::stack<uint16_t> STACK;

	/*High res display*/
	uint8_t DISPLAY[64 * 32];
};

