// Chip8Emulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Chippy.hpp"

int main(int argc, char** argv)
{
	// Check arguments
	if (argc != 2)
	{
		std::cout << "usage: chippy <program>" << std::endl;
		return 1;
	}

	// Try to open file
	std::ifstream file;
	file.open(argv[1], std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		std::cout << "Failed to open program file" << std::endl;
		return 1;
	}

	// Initialize
	Chippy* cpuTest = new Chippy();

	// Load file into temp buffer
	std::streampos size = file.tellg();
	char* buffer = new char[size];
	file.seekg(0, std::ios::beg);
	file.read(buffer, size);
	file.close();

	// Load temp buffer into memory
	cpuTest->load(buffer, size);

	// Cleanup
	delete[] buffer;

	// Our program loooooop
	SetTargetFPS(60);

	while (!cpuTest->ShouldClose())
	{
		// Execute specified amount of instructions per second
		for (int i = 0; i < 10; i++)
		{
			cpuTest->set_key();
			cpuTest->opcode();
		}

		// Update timers
		cpuTest->update_timers();

		// Update display
		// TODO: Implement display
	}

	return 0;
}