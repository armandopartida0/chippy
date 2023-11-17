#include "chippy_input.hpp"

std::array<int, 16>& ChippyInput::GetInput()
{
  /* Wipe our keyboard first */
  std::fill(std::begin(keyboard_), std::end(keyboard_), 0);

  /* Get all current key states and set keys accordingly */
	const Uint8* current_key_states = SDL_GetKeyboardState(nullptr);

	if (current_key_states[SDL_SCANCODE_X])
	{
		keyboard_[0] = 1;
	}
	if (current_key_states[SDL_SCANCODE_1])
	{
		keyboard_[1] = 1;
	}
	if (current_key_states[SDL_SCANCODE_2])
	{
		keyboard_[2] = 1;
	}
	if (current_key_states[SDL_SCANCODE_3])
	{
		keyboard_[3] = 1;
	}
	if (current_key_states[SDL_SCANCODE_Q])
	{
		keyboard_[4] = 1;
	}
	if (current_key_states[SDL_SCANCODE_W])
	{
		keyboard_[5] = 1;
	}
	if (current_key_states[SDL_SCANCODE_E])
	{
		keyboard_[6] = 1;
	}
	if (current_key_states[SDL_SCANCODE_A])
	{
		keyboard_[7] = 1;
	}
	if (current_key_states[SDL_SCANCODE_S])
	{
		keyboard_[8] = 1;
	}
	if (current_key_states[SDL_SCANCODE_D])
	{
		keyboard_[9] = 1;
	}
	if (current_key_states[SDL_SCANCODE_Z])
	{
		keyboard_[0xA] = 1;
	}
	if (current_key_states[SDL_SCANCODE_C])
	{
		keyboard_[0xB] = 1;
	}
	if (current_key_states[SDL_SCANCODE_4])
	{
		keyboard_[0xC] = 1;
	}
	if (current_key_states[SDL_SCANCODE_R])
	{
		keyboard_[0xD] = 1;
	}
	if (current_key_states[SDL_SCANCODE_F])
	{
		keyboard_[0xE] = 1;
	}
	if(current_key_states[SDL_SCANCODE_V])
	{
		keyboard_[0xF] = 1;
	}

  return keyboard_;
}