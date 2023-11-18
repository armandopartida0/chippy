#include "raylib.h"

#include "chippy_input.hpp"

std::array<int, 16> &ChippyInput::GetInput()
{
	/* Wipe our keyboard first */
	// std::fill(std::begin(keyboard_), std::end(keyboard_), 0);

	if (IsKeyPressed(KeyboardKey::KEY_X))
	{
		keyboard_[0] = 1;
	}
	if (IsKeyPressed(KeyboardKey::KEY_KP_1))
	{
		keyboard_[1] = 1;
	}
	if (IsKeyPressed(KeyboardKey::KEY_KP_2))
	{
		keyboard_[2] = 1;
	}
	if (IsKeyPressed(KeyboardKey::KEY_KP_3))
	{
		keyboard_[3] = 1;
	}
	if (IsKeyPressed(KeyboardKey::KEY_Q))
	{
		keyboard_[4] = 1;
	}
	if (IsKeyPressed(KeyboardKey::KEY_W))
	{
		keyboard_[5] = 1;
	}
	if (IsKeyPressed(KeyboardKey::KEY_E))
	{
		keyboard_[6] = 1;
	}
	if (IsKeyPressed(KeyboardKey::KEY_A))
	{
		keyboard_[7] = 1;
	}
	if (IsKeyPressed(KeyboardKey::KEY_S))
	{
		keyboard_[8] = 1;
	}
	if (IsKeyPressed(KeyboardKey::KEY_D))
	{
		keyboard_[9] = 1;
	}
	if (IsKeyPressed(KeyboardKey::KEY_Z))
	{
		keyboard_[0xA] = 1;
	}
	if (IsKeyPressed(KeyboardKey::KEY_C))
	{
		keyboard_[0xB] = 1;
	}
	if (IsKeyPressed(KeyboardKey::KEY_KP_4))
	{
		keyboard_[0xC] = 1;
	}
	if (IsKeyPressed(KeyboardKey::KEY_R))
	{
		keyboard_[0xD] = 1;
	}
	if (IsKeyPressed(KeyboardKey::KEY_F))
	{
		keyboard_[0xE] = 1;
	}
	if (IsKeyPressed(KeyboardKey::KEY_V))
	{
		keyboard_[0xF] = 1;
	}

	return keyboard_;
}