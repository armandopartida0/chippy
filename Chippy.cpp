#include "Chippy.h"

Chippy::Chippy()
{
	// initialize stuff here
	PC = 0x200;
	I = 0;

	for (int i = 0; i < 4096; i++)
	{
		MEMORY[i] = 0;
	}
}

void Chippy::opcode()
{
	// Reference: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM 
	// All instructions are two bytes. So increase PC by 2 unless specified
	uint16_t instruction = MEMORY[PC] << 8 | MEMORY[PC + 1];
	switch (instruction & 0xF000)
	{
	case 0x0000:
	{
		switch (instruction)
		{
		case 0x00E0: // Clear the display.
			PC += 2;
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
		if (V[(instruction >> 8) & 0x000F] == (instruction & 0x00FF))
		{
			PC += 4;
		}
		else
		{
			PC += 2;
		}
		break;
	case 0x4000: // Skip next instruction if Vx != kk.
		if (V[(instruction >> 8) & 0x000F] != (instruction & 0x00FF))
		{
			PC += 4;
		}
		else
		{
			PC += 2;
		}
		break;
	case 0x5000: // Skip next instruction if Vx = Vy.
		if (V[(instruction >> 8) & 0x000F] == V[(instruction >> 4) & 0x000F])
		{
			PC += 4;
		}
		else
		{
			PC += 2;
		}
		break;
	case 0x6000: // Set Vx = kk.
		V[(instruction >> 8) & 0x000F] = instruction & 0x00FF;
		break;
	case 0x7000: // Set Vx = Vx + kk.
		V[(instruction >> 8) & 0x000F] += instruction & 0x00FF;
		break;
	case 0x8000: // 0x8xy0 - 0x8xy7, 0x8xyE
	{
		uint16_t lowByte = MEMORY[PC] << 12;

		switch (lowByte)
		{
		case 0x0000: // Set Vx = Vy.
			V[(instruction >> 8) & 0x000F] = V[(instruction >> 4) & 0x000F];
			break;
		case 0x1000: // Set Vx = Vx OR Vy.
			V[(instruction >> 8) & 0x000F] = V[(instruction >> 8) & 0x000F] | V[(instruction >> 4) & 0x000F];
			break;
		case 0x2000: // Set Vx = Vx AND Vy.
			V[(instruction >> 8) & 0x000F] = V[(instruction >> 8) & 0x000F] & V[(instruction >> 4) & 0x000F];
			break;
		case 0x3000: // Set Vx = Vx XOR Vy.
			V[(instruction >> 8) & 0x000F] = V[(instruction >> 8) & 0x000F] ^ V[(instruction >> 4) & 0x000F];
			break;
		case 0x5000: // Set Vx = Vx - Vy, set VF = NOT borrow.
			if (V[(instruction >> 8) & 0x000F] > V[(instruction >> 4) & 0x000F])
			{
				V[0xF] = 1;
			}
			else
			{
				V[0xF] = 0;
			}

			V[(instruction >> 8) & 0x000F] -= V[(instruction >> 4) & 0x000F];
			break;
		}

		PC += 2;

		break;
	}

	default:
		std::cout << "Instruction: " << std::hex << MEMORY[PC] << " not implemented." << std::endl;
	}
}
