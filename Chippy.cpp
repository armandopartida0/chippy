#include "Chippy.h"

Chippy::Chippy()
{
	// initialize stuff here
}

void Chippy::opcode()
{
	uint16_t highByte = PC & 0xF000;
	switch (highByte)
	{
	case 0x0000:
	{
		switch (PC)
		{
		case 0x00E0: // Clear the display.
			break;
		case 0x00EE: // Return from a subroutine.
			PC = STACK.top();
			STACK.pop();
			break;
		}
		break;
	}
	case 0x1000: // Jump to location nnn.
		PC = PC & 0x0FFF;
		break;
	case 0x2000: // Call subroutine at nnn.
		STACK.push(PC);
		PC = PC & 0x0FFF;
		break;
	case 0x3000: // Skip next instruction if Vx = kk.
		if (V[(PC >> 8) & 0x000F] == (PC & 0x00FF))
		{
			PC += 2;
		}
		break;
	case 0x4000: // Skip next instruction if Vx != kk.
		if (V[(PC >> 8) & 0x000F] != (PC & 0x00FF))
		{
			PC += 2;
		}
		break;
	case 0x5000: // Skip next instruction if Vx = Vy.
		if (V[(PC >> 8) & 0x000F] == V[(PC >> 4) & 0x000F])
		{
			PC += 2;
		}
		break;
	case 0x6000: // Set Vx = kk.
		V[(PC >> 8) & 0x000F] = PC & 0x00FF;
		break;
	case 0x7000: // Set Vx = Vx + kk.
		V[(PC >> 8) & 0x000F] += PC & 0x00FF;
		break;
	case 0x8000:
	{

	}
	}
}
