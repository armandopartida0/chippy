#ifndef CHIPPY_DISPLAY_H
#define CHIPPY_DISPLAY_H

#include <array>
#include <cstdint>
#include <memory>

#include "SDL.h"

class ChippyDisplay
{
public:
  ChippyDisplay();
  ~ChippyDisplay();

  void Draw(std::array<uint32_t, 64 * 32> buffer);
  inline bool IsWindowRunning() const { return !WindowShouldClose(); }

private:
  /* Window settings */
  const int kwindow_width_ = 1280;
  const int kwindow_height_ = 640;

  /* SDL stuff */
  std::unique_ptr<
};

#endif