#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

/**
 * @brief Here we define settings used throughout the program.
 *
 */

// Window settings
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 640;

// Chip-8 configuration
const unsigned int INTERNAL_WIDTH = 64;  // WARNING: Probably shouldn't change this!
const unsigned int INTERNAL_HEIGHT = 32; // WARNING: Probably shouldn't change this!
const unsigned int PROGRAM_START = 0x200;
const unsigned int SPRITE_SET_START = 0x10;

#endif