#include <fstream>
#include <iostream>
#include <memory>

#include "chippy_cpu.hpp"
#include "chippy_display.hpp"
#include "chippy_input.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "usage: chippy <program> \n";
		return 0;
	}

	/* Try to open file */
	std::ifstream file;
	file.open(argv[1], std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		std::cout << "Failed to open program file \n";
		return 1;
	}

	/* Initialize */
	std::unique_ptr<ChippyCpu> cpu = std::make_unique<ChippyCpu>();
	std::unique_ptr<ChippyInput> input = std::make_unique<ChippyInput>();
	std::unique_ptr<ChippyDisplay> display = std::make_unique<ChippyDisplay>();

	/* Load file into temp buffer */
	std::streampos size = file.tellg();
	char *buffer = new char[size];
	file.seekg(0, std::ios::beg);
	file.read(buffer, size);
	file.close();

	/* Load temp buffer into memory */
	cpu->LoadProgram(buffer, static_cast<size_t>(size));

	/* Cleanup */
	delete[] buffer;

	SetTargetFPS(60);
	while (display->IsWindowRunning())
	{
		

		/* Roughly 8 instructions per frame */
		for (auto i = 0; i < 8; i++)
		{
			cpu->SetKeyboardState(input->GetInput());
			cpu->Opcode();
		}

		cpu->UpdateTimers();
		display->Draw(cpu->GetDisplayBuffer());
		// input->ClearInputs();
	}

	return 0;
}