#include <fstream>
#include <iostream>
#include <memory>

#include "SDL.h"

#include "chippy_cpu.h"
#include "chippy_display.h"
#include "chippy_input.h"

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
	std::unique_ptr<ChippyInput> input = std::make_unique<ChippyInput>();
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
	
	/* Loop will try to run at 60 FPS, not accurate because of sleep */
	bool quit = false;
	SDL_Event e;
	Uint32 time_per_frame = 1000 / 60;
	while (true)
	{
		auto start_time = SDL_GetTicks();

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				return 0;
			}
		}

		/* Roughly 8 instructions per frame */
		for(auto i = 0; i < 8; i++)
		{
			cpu->SetKeyboardState(input->GetInput());
			cpu->Opcode();
		}

		auto end_time = SDL_GetTicks();
		auto delta_time = end_time - start_time;
		SDL_Delay(time_per_frame - delta_time);

		cpu->UpdateTimers();
		display->Draw(cpu->GetDisplayBuffer());
	}
}