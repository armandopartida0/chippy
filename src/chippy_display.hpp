#ifndef CHIPPY_DISPLAY_HPP
#define CHIPPY_DISPLAY_HPP

#include <array>
#include <cstdint>
#include <iostream>
#include <memory>

#include "raylib.h"

#include "configuration.hpp"

class ChippyDisplay
{
public:
  ChippyDisplay();
  ~ChippyDisplay();

  void Draw(const std::array<std::uint32_t, INTERNAL_WIDTH * INTERNAL_HEIGHT> &buffer) const;
  bool IsWindowRunning() const { return !WindowShouldClose(); }

private:
  /* raylib GPU texture */
  Texture2D texture_;
};

#endif