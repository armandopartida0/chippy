#include "chippy_cpu.hpp"

ChippyCpu::ChippyCpu()
{
	/* Initialize chip-8 stuff here */
	pc_ = kprogram_start_;
	i_register_ = 0;
	delay_timer_ = 0;
	sound_timer_ = 0;
	std::srand(static_cast<unsigned int>(time(0)));

	/* Clear data */
	std::fill(std::begin(display_), std::end(display_), 0);
	std::fill(std::begin(memory_), std::end(memory_), 0);
	std::fill(std::begin(v_register_), std::end(v_register_), 0);

	/* Load spriteset */
	for (auto i = 0; i < ksprites_.size(); i++)
	{
		memory_[ksprite_set_start_ + i] = ksprites_[i];
	}
}

ChippyCpu::~ChippyCpu()
{

}

void ChippyCpu::LoadProgram(char* buffer, std::streampos size)
{
	for (auto i = 0; i < size; i++)
	{
		memory_[i + kprogram_start_] = buffer[i];
	}
}

void ChippyCpu::SetKeyboardState(std::array<int, 16> state)
{
	keyboard_ = state;
}

void ChippyCpu::Opcode()
{
	/* Reference: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM */
	/* All instructions are two bytes. So increase PC by 2 unless specified */
	std::uint16_t instruction = static_cast<std::uint16_t>(memory_[pc_] << 8 | memory_[pc_ + 1]);

	/* Common variables */
	int x = (instruction & 0x0F00) >> 8; /* Index used for V[x] register */
	int y = (instruction & 0x00F0) >> 4; /* Index used for V[y] register */

	pc_ += 2;

	switch (instruction & 0xF000)
	{
		case 0x0000:
		{
			switch (instruction)
			{
				case 0x00E0: /* Clear the display. */
					std::fill(std::begin(display_), std::end(display_), 0);
					break;
				case 0x00EE: /* Return from a subroutine. */
					pc_ = stack_.top();
					stack_.pop();
					break;
			}
			break;
		}
		case 0x1000: /* Jump to location nnn. */
			pc_ = instruction & 0x0FFF;
			break;
		case 0x2000: /* Call subroutine at nnn. */
			stack_.push(pc_);
			pc_ = instruction & 0x0FFF;
			break;
		case 0x3000: /* Skip next instruction if Vx = kk. */
			if (v_register_[x] == (instruction & 0x00FF))
			{
				pc_ += 2;
			}
			break;
		case 0x4000: /* Skip next instruction if Vx != kk. */
			if (v_register_[x] != (instruction & 0x00FF))
			{
				pc_ += 2;
			}
			break;
		case 0x5000: /* Skip next instruction if Vx = Vy. */
			if (v_register_[x] == v_register_[y])
			{
				pc_ += 2;
			}
			break;
		case 0x6000: /* Set Vx = kk. */
			v_register_[x] = instruction & 0x00FF;
			break;
		case 0x7000: /* Set Vx = Vx + kk. */
			v_register_[x] += instruction & 0x00FF;
			break;
		case 0x8000: /* 0x8xy0 - 0x8xy7, 0x8xyE */
		{
			switch (instruction & 0x000F)
			{
				case 0x0000: /* Set Vx = Vy. */
					v_register_[x] = v_register_[y];
					break;
				case 0x0001: /* Set Vx = Vx OR Vy. */
					v_register_[x] = v_register_[x] | v_register_[y];
					break;
				case 0x0002: /* Set Vx = Vx AND Vy. */
					v_register_[x] = v_register_[x] & v_register_[y];
					break;
				case 0x0003: /* Set Vx = Vx XOR Vy. */
					v_register_[x] = v_register_[x] ^ v_register_[y];
					break;
				case 0x0004: /* Set Vx = Vx + Vy, set VF = carry. */
				{
					std::uint16_t sum = static_cast<std::uint16_t>(v_register_[x] + v_register_[y]);

					if (sum > 255)
					{
						v_register_[0xF] = 1;
					}
					else
					{
						v_register_[0xF] = 0;
					}

					v_register_[x] = sum & 0x00FF;

					break;
				}
				case 0x0005: /* Set Vx = Vx - Vy, set VF = NOT borrow. */
					if (v_register_[x] > v_register_[y])
					{
						v_register_[0xF] = 1;
					}
					else
					{
						v_register_[0xF] = 0;
					}

					v_register_[x] -= v_register_[y];
					break;
				case 0x0006: /* Set Vx = Vx SHR 1. */
					if (v_register_[x] & 0x1)
					{
						v_register_[0xF] = 1;
					}
					else
					{
						v_register_[0xF] = 0;
					}
					v_register_[x] /= 2;
					break;
				case 0x0007: /* Set Vx = Vy - Vx, set VF = NOT borrow. */
					if (v_register_[y] > v_register_[x])
					{
						v_register_[0xF] = 1;
					}
					else
					{
						v_register_[0xF] = 0;
					}
					v_register_[x] = v_register_[y] - v_register_[x];
					break;
				case 0x000E: /* Set Vx = Vx SHL 1. */
					if (v_register_[x] & 0x80) /* Any number other than 0 is true */
					{
						v_register_[0xF] = 1;
					}
					else
					{
						v_register_[0xF] = 0;
					}
					v_register_[x] *= 2;
					break;
				default:
					std::cout << "Instruction: " << std::hex << instruction << " not implemented. \n";
			}
			break;
		}
		case 0x9000: /* Skip next instruction if Vx != Vy. */
			if (v_register_[x] != v_register_[y])
			{
				pc_ += 2;
			}
			break;
		case 0xA000: /* Set I = nnn. */
			i_register_ = instruction & 0x0FFF;
			break;
		case 0xB000: /* Jump to location nnn + V0. */
			pc_ = (instruction & 0x0FFF) + v_register_[0];
			break;
		case 0xC000: /* Set Vx = random byte AND kk. */
			v_register_[x] = static_cast<int>(std::rand() % 255 + 1) & (instruction & 0x00FF);
			break;
		case 0xD000: /* Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision. */
		{
			/* Make sure it wraps with display */
			std::uint8_t x_pos = static_cast<std::uint8_t>(v_register_[x] % kdisplay_width_);
			std::uint8_t y_pos = static_cast<std::uint8_t>(v_register_[y] % kdisplay_height_);
			std::uint8_t height = static_cast<std::uint8_t>(instruction & 0x000F);

			v_register_[0xF] = 0;

			for (auto i = 0; i < height; i++)
			{
				std::uint8_t sprite_data = memory_[i_register_ + i];

				/* All sprites are 8 bits in width */
				for (auto j = 0; j < 8; j++)
				{
					std::uint8_t sprite_pixel = static_cast<std::uint8_t>(sprite_data & (0x80 >> j));

					/* Get the corresponding display pixel on our screen */
					std::uint32_t* display_pixel = &display_.data()[(y_pos + i) * kdisplay_width_ + (x_pos + j)];

					/* Check if they collide */
					if (sprite_pixel)
					{
						if (*display_pixel == 0xFFFFFFFF)
						{
							v_register_[0xF] = 1;
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
				case 0x009E: /* Skip next instruction if key with the value of Vx is pressed. */
					if (keyboard_[v_register_[x]])
					{
						pc_ += 2;
					}
					break;
				case 0x00A1: /* Skip next instruction if key with the value of Vx is not pressed. */
					if (!keyboard_[v_register_[x]])
					{
						pc_ += 2;
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
				case 0x0007: /* Set Vx = delay timer value. */
					v_register_[x] = delay_timer_;
					break;
				case 0x000A: /* Wait for a key press, store the value of the key in Vx. */
				{
					bool key_pressed{ false };
					for (auto i = 0; i < keyboard_.size(); i++)
					{
						if (keyboard_[i])
						{
							key_pressed = true;
							v_register_[x] = i;
						}
					}

					if (!key_pressed)
					{
						pc_ -= 2; /* This is so that we stay on the same instruction */
					}
					break;
				}
				case 0x0015: /* Set delay timer = Vx. */
					delay_timer_ = v_register_[x];
					break;
				case 0x0018: /* Set sound timer = Vx. */
					sound_timer_ = v_register_[x];
					break;
				case 0x001E: /* Set I = I + Vx. */
					i_register_ += v_register_[x];
					break;
				case 0x0029: /* Set I = location of sprite for digit Vx. */
					i_register_ = (v_register_[x] * 5) + ksprite_set_start_;
					break;
				case 0x0033: /* Store BCD representation of Vx in memory locations I, I+1, and I+2. */
					memory_[i_register_ + 2] = v_register_[x] % 10;
					memory_[i_register_ + 1] = (v_register_[x] / 10) % 10;
					memory_[i_register_] = (v_register_[x] / 100) % 10;
					break;
				case 0x0055: /* Store registers V0 through Vx in memory starting at location I. */
					for (auto i = 0; i <= x; i++)
					{
						memory_[i_register_ + i] = v_register_[i];
					}
					break;
				case 0x0065: /* Read registers V0 through Vx from memory starting at location I. */
					for (auto i = 0; i <= x; i++)
					{
						v_register_[i] = memory_[i_register_ + i];
					}
					break;
			}
			break;
		}
		default:
			std::cout << "Instruction: " << std::hex << instruction << " not implemented. \n";
	}

	/* std::cout << "Instruction: " << std::hex << instruction << " executed. \n"; */
}

void ChippyCpu::UpdateTimers()
{
	/* Timers: Should subtract 1 from values at 60hz */
	if (delay_timer_ > 0)
	{
		delay_timer_--;
	}
	if (sound_timer_ > 0)
	{
		sound_timer_--;
	}
}