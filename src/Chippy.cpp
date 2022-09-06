#include "Chippy.hpp"

Chippy::Chippy()
{
	// initialize chip8 stuff here
	PC = PROGRAM_START;
	I = 0;
	DELAY_TIMER = 0;
	SOUND_TIMER = 0;
	std::srand(static_cast<unsigned int>(time(0)));

	// Clear data
	std::fill(std::begin(DISPLAY), std::end(DISPLAY), 0);
	std::fill(std::begin(MEMORY), std::end(MEMORY), 0);
	std::fill(std::begin(V), std::end(V), 0);

	// Load spriteset
	for (int i = 0; i < sprites.size(); i++)
	{
		MEMORY[SPRITESET_START + i] = sprites[i];
	}

	// initialize SDL stuff
	/*if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "Failed to initialize SDL Video subsystem" << std::endl;
	}
	else
	{
		window = SDL_CreateWindow("chippy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 640, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	}*/
}

Chippy::~Chippy()
{
	// Cleanup SDL stuff
	/*SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();*/
}

void Chippy::load(char* buffer, std::streampos size)
{
	for (long i = 0; i < size; i++)
	{
		MEMORY[i + PROGRAM_START] = buffer[i];
	}
}

void Chippy::set_key()
{
	// Clear our keyboard
	std::fill(std::begin(keyboard), std::end(keyboard), 0);

	/*// Get all current key states and set keys accordingly
	const std::uint8_t* current_key_states{ SDL_GetKeyboardState(nullptr) };

	if (current_key_states[SDL_SCANCODE_X])
	{
		keyboard[0] = 1;
	}
	if (current_key_states[SDL_SCANCODE_1])
	{
		keyboard[1] = 1;
	}
	if (current_key_states[SDL_SCANCODE_2])
	{
		keyboard[2] = 1;
	}
	if (current_key_states[SDL_SCANCODE_3])
	{
		keyboard[3] = 1;
	}
	if (current_key_states[SDL_SCANCODE_Q])
	{
		keyboard[4] = 1;
	}
	if (current_key_states[SDL_SCANCODE_W])
	{
		keyboard[5] = 1;
	}
	if (current_key_states[SDL_SCANCODE_E])
	{
		keyboard[6] = 1;
	}
	if (current_key_states[SDL_SCANCODE_A])
	{
		keyboard[7] = 1;
	}
	if (current_key_states[SDL_SCANCODE_S])
	{
		keyboard[8] = 1;
	}
	if (current_key_states[SDL_SCANCODE_D])
	{
		keyboard[9] = 1;
	}
	if (current_key_states[SDL_SCANCODE_Z])
	{
		keyboard[0xA] = 1;
	}
	if (current_key_states[SDL_SCANCODE_C])
	{
		keyboard[0xB] = 1;
	}
	if (current_key_states[SDL_SCANCODE_4])
	{
		keyboard[0xC] = 1;
	}
	if (current_key_states[SDL_SCANCODE_R])
	{
		keyboard[0xD] = 1;
	}
	if (current_key_states[SDL_SCANCODE_F])
	{
		keyboard[0xE] = 1;
	}
	if(current_key_states[SDL_SCANCODE_V])
	{
		keyboard[0xF] = 1;
	}*/
}

void Chippy::opcode()
{
	// Reference: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM 
	// All instructions are two bytes. So increase PC by 2 unless specified
	std::uint16_t instruction{ static_cast<std::uint16_t>(MEMORY[PC] << 8 | MEMORY[PC + 1]) };

	// Common variables
	int x{ (instruction & 0x0F00) >> 8 }; // Index used for V[x] register
	int y{ (instruction & 0x00F0) >> 4 }; // Index used for V[y] register

	PC += 2;

	switch (instruction & 0xF000)
	{
	case 0x0000:
	{
		switch (instruction)
		{
		case 0x00E0: // Clear the display.
			std::fill(std::begin(DISPLAY), std::end(DISPLAY), 0);
			break;
		case 0x00EE: // Return from a subroutine.
			PC = STACK.top();
			STACK.pop();
			break;
		}
		break;
	}
	case 0x1000: // Jump to location nnn.
		PC = instruction & 0x0FFF;
		break;
	case 0x2000: // Call subroutine at nnn.
		STACK.push(PC);
		PC = instruction & 0x0FFF;
		break;
	case 0x3000: // Skip next instruction if Vx = kk.
		if (V[x] == (instruction & 0x00FF))
		{
			PC += 2;
		}
		break;
	case 0x4000: // Skip next instruction if Vx != kk.
		if (V[x] != (instruction & 0x00FF))
		{
			PC += 2;
		}
		break;
	case 0x5000: // Skip next instruction if Vx = Vy.
		if (V[x] == V[y])
		{
			PC += 2;
		}
		break;
	case 0x6000: // Set Vx = kk.
		V[x] = instruction & 0x00FF;
		break;
	case 0x7000: // Set Vx = Vx + kk.
		V[x] += instruction & 0x00FF;
		break;
	case 0x8000: // 0x8xy0 - 0x8xy7, 0x8xyE
	{
		switch (instruction & 0x000F)
		{
		case 0x0000: // Set Vx = Vy.
			V[x] = V[y];
			break;
		case 0x0001: // Set Vx = Vx OR Vy.
			V[x] = V[x] | V[y];
			break;
		case 0x0002: // Set Vx = Vx AND Vy.
			V[x] = V[x] & V[y];
			break;
		case 0x0003: // Set Vx = Vx XOR Vy.
			V[x] = V[x] ^ V[y];
			break;
		case 0x0004: // Set Vx = Vx + Vy, set VF = carry.
		{
			std::uint16_t sum{ static_cast<std::uint16_t>(V[x] + V[y]) };

			if (sum > 255)
			{
				V[0xF] = 1;
			}
			else
			{
				V[0xF] = 0;
			}

			V[x] = sum & 0x00FF;

			break;
		}
		case 0x0005: // Set Vx = Vx - Vy, set VF = NOT borrow.
			if (V[x] > V[y])
			{
				V[0xF] = 1;
			}
			else
			{
				V[0xF] = 0;
			}

			V[x] -= V[y];
			break;
		case 0x0006: // Set Vx = Vx SHR 1.
			if (V[x] & 0x1)
			{
				V[0xF] = 1;
			}
			else
			{
				V[0xF] = 0;
			}
			V[x] /= 2;
			break;
		case 0x0007: // Set Vx = Vy - Vx, set VF = NOT borrow.
			if (V[y] > V[x])
			{
				V[0xF] = 1;
			}
			else
			{
				V[0xF] = 0;
			}
			V[x] = V[y] - V[x];
			break;
		case 0x000E: // Set Vx = Vx SHL 1.
			if (V[x] & 0x80) // Any number other than 0 is true
			{
				V[0xF] = 1;
			}
			else
			{
				V[0xF] = 0;
			}
			V[x] *= 2;
			break;
		default:
			std::cout << "Instruction: " << std::hex << instruction << " not implemented." << std::endl;
		}
		break;
	}
	case 0x9000: // Skip next instruction if Vx != Vy.
		if (V[x] != V[y])
		{
			PC += 2;
		}
		break;
	case 0xA000: // Set I = nnn.
		I = instruction & 0x0FFF;
		break;
	case 0xB000: // Jump to location nnn + V0.
		PC = (instruction & 0x0FFF) + V[0];
		break;
	case 0xC000: // Set Vx = random byte AND kk.
		V[x] = static_cast<int>(std::rand() % 255 + 1) & (instruction & 0x00FF);
		break;
	case 0xD000: // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
	{
		// Make sure it wraps with display
		std::uint8_t x_pos{ static_cast<std::uint8_t>(V[x] % DISPLAY_WIDTH) };
		std::uint8_t y_pos{ static_cast<std::uint8_t>(V[y] % DISPLAY_HEIGHT) };
		std::uint8_t height{ static_cast<std::uint8_t>(instruction & 0x000F) };

		V[0xF] = 0;

		for (int i = 0; i < height; i++)
		{
			std::uint8_t sprite_data{ MEMORY[I + i] };

			// All sprites are 8 bits in width
			for (int j = 0; j < 8; j++)
			{
				std::uint8_t sprite_pixel{ static_cast<std::uint8_t>(sprite_data & (0x80 >> j)) };

				// Get the corresponding display pixel on our screen
				std::uint32_t* display_pixel{ &DISPLAY.data()[(y_pos + i) * DISPLAY_WIDTH + (x_pos + j)] };

				// Check if they collide
				if (sprite_pixel)
				{
					if (*display_pixel == 0xFFFFFFFF)
					{
						V[0xF] = 1;
					}

					*display_pixel ^= 0xFFFFFFFF;
				}
			}
		}
		break;
	}
	case 0xE000:
	{
		switch (instruction & 0x00FF)
		{
		case 0x009E: // Skip next instruction if key with the value of Vx is pressed.
			if (keyboard[V[x]])
			{
				PC += 2;
			}
			break;
		case 0x00A1: // Skip next instruction if key with the value of Vx is not pressed.
			if (!keyboard[V[x]])
			{
				PC += 2;
			}
			break;
		default:
			break;
		}
	}
	case 0xF000:
	{
		switch (instruction & 0x00FF)
		{
		case 0x0007: // Set Vx = delay timer value.
			V[x] = DELAY_TIMER;
			break;
		case 0x000A: // Wait for a key press, store the value of the key in Vx.
		{
			bool key_pressed{ false };
			for (int i = 0; i < keyboard.size(); i++)
			{
				if (keyboard[i])
				{
					key_pressed = true;
					V[x] = i;
				}
			}

			if (!key_pressed)
			{
				PC -= 2; // This is so that we stay on the same instruction
			}
			break;
		}
		case 0x0015: // Set delay timer = Vx.
			DELAY_TIMER = V[x];
			break;
		case 0x0018: // Set sound timer = Vx.
			SOUND_TIMER = V[x];
			break;
		case 0x001E: // Set I = I + Vx.
			I += V[x];
			break;
		case 0x0029: // Set I = location of sprite for digit Vx.
			I = (V[x] * 5) + SPRITESET_START;
			break;
		case 0x0033: // Store BCD representation of Vx in memory locations I, I+1, and I+2.
			MEMORY[I + 2] = V[x] % 10;
			MEMORY[I + 1] = (V[x] / 10) % 10;
			MEMORY[I] = (V[x] / 100) % 10;
			break;
		case 0x0055: // Store registers V0 through Vx in memory starting at location I.
			for (int i = 0; i <= x; i++)
			{
				MEMORY[I + i] = V[i];
			}
			break;
		case 0x0065: // Read registers V0 through Vx from memory starting at location I.
			for (int i = 0; i <= x; i++)
			{
				V[i] = MEMORY[I + i];
			}
			break;
		}
		break;
	}
	default:
		std::cout << "Instruction: " << std::hex << instruction << " not implemented." << std::endl;
	}

	//std::cout << "Instruction: " << std::hex << instruction << " executed." << std::endl;
}

void Chippy::update_timers()
{
	// Timers: Should subtract 1 from values at 60hz
	if (DELAY_TIMER > 0)
	{
		DELAY_TIMER--;
	}
	if (SOUND_TIMER > 0)
	{
		SOUND_TIMER--;
	}
}

void Chippy::display()
{
	/*SDL_UpdateTexture(texture, nullptr, DISPLAY.data(), sizeof(DISPLAY[0]) * DISPLAY_WIDTH);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);*/
}