#ifndef CHIPPY_CPU_HPP
#define CHIPPY_CPU_HPP

#include <array>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <fstream>
#include <memory>
#include <stack>
#include <thread>

class ChippyCpu
{
public:
	ChippyCpu(); /* Initialize chip-8 state, load sprite data */

	void LoadProgram(char* buffer, std::streampos size); /* Loads in program into memory */
	void SetKeyboardState(std::array<int, 16> state); /* Sets currently pressed key into memory */
	void Opcode(); /* Executes single opcode, changes memory state */
	void UpdateTimers(); /* Updates our chip-8 timers */
	inline std::array<uint32_t, 64 * 32> GetDisplayBuffer() const { return display_; }

	~ChippyCpu();

private:
	/*
		Memory Map:
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
		+---------------+= 0x000 (0) Start of Chip-8 RAM
	*/
	std::array<std::uint8_t, 4096> memory_{};
	const int kprogram_start_ = 0x200;

	/* 
		I'm not sure where exactly it should be started other than that
		it should be between 0x000-0x1FF, so lets just put it near the beginning.
		Also there are 16 sprites, each 5 bytes in size
		Reference (Display): http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
	*/
	const int ksprite_set_start_ = 0x10;
	const std::array<std::uint8_t, 16 * 5> ksprites_
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
		0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
		0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
		0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
		0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
		0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
		0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
		0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
		0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
		0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
		0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
		0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
		0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
		0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
		0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
		0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
	};

	/* 
		Our keyboard: 
		If a key at any index is 1, then that key is pressed; otherwise not pressed.
		Layout of chippy's keyboard
		Of course we map it differently. I have 60% keyboard :(
			_ _ _ _		 _ _ _ _
			|1|2|3|C|		|1|2|3|4|
			|4|5|6|D|		|Q|W|E|R|
			|7|8|9|E| ->|A|S|D|F|
			|A|0|B|F|		|Z|X|C|V|
			- - - -		 - - - -
	*/
	std::array<int, 16> keyboard_{};

	/* Registers */
	std::array<std::uint8_t, 16> v_register_{}; /* 16 general 8-bit registers V0-VF */
	std::uint16_t i_register_{}; /* 16-bit register to store memory addresses, lowest 12 bits are used */

	/* Timers, rate of 60hz */
	std::uint8_t delay_timer_{};
	std::uint8_t sound_timer_{};

	/* pseudo-regsssss */
	std::uint16_t pc_{}; /* program counter */

	/* Big fat stack */
	std::stack<std::uint16_t> stack_{};

	/* High-res display lol */
	const int kdisplay_width_ = 64;
	const int kdisplay_height_ = 32;
	std::array<uint32_t, 64 * 32> display_{};
};

#endif