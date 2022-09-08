#include <fstream>
#include <iostream>
#include <memory>

#include "SDL.h"

#include "chippy_cpu.h"
#include "chippy_display.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "usage: chippy <program> \n";
		return 1;
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
	std::unique_ptr<ChippyDisplay> display = std::make_unique<ChippyDisplay>();

	/* Load file into temp buffer */
	std::streampos size = file.tellg();
	char *buffer = new char[size];
	file.seekg(0, std::ios::beg);
	file.read(buffer, size);
	file.close();

	/* Load temp buffer into memory */
	cpu->LoadProgram(buffer, size);

	/* Cleanup */
	delete[] buffer;

	bool quit = false;
	SDL_Event e;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			// Quit on pressing x button
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		/* Execute specified amount of instructions per second */
		for (int i = 0; i < 10; i++)
		{
			cpu->SetKey();
			cpu->Opcode();
		}

		/* Update timers */
		cpu->UpdateTimers();

		/* Update display */
		display->Draw(cpu->GetDisplayBuffer());
	}

	return 0;
}