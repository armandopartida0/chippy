#ifndef CHIPPY_DISPLAY_HPP
#define CHIPPY_DISPLAY_HPP

#include <array>
#include <cstdint>
#include <iostream>
#include <memory>

#include "raylib.h"

class ChippyDisplay
{
public:
  ChippyDisplay();
  ~ChippyDisplay();

  void Draw(std::array<uint32_t, 64 * 32> buffer);
  inline bool IsWindowRunning() const { return !WindowShouldClose(); }

private:
  /* Window settings */
  const unsigned int WINDOW_WIDTH = 1280;
  const unsigned int WINDOW_HEIGHT = 640;

  /* SDL stuff */
  Texture2D texture_;
};

#endif