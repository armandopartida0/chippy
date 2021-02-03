// Chip8Emulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Chippy.h"

int main()
{
	// Verifying number stuff
	/*uint16_t number = 0x8232;

	uint16_t lowByte = number << 12;
	uint16_t highByte = number & 0xF000;

	if (highByte == 0x8000)
	{
		std::cout << "True" << std::endl;
	}*/

	Chippy* cpuTest = new Chippy();
}