#include "chippy_display.hpp"

ChippyDisplay::ChippyDisplay()
{
  // Configure window
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "chippy");
  SetConfigFlags(FLAG_VSYNC_HINT);

  // Setup internal display (Just a scaled up texture)
  Image image = GenImageColor(64, 32, BLACK);
  texture_ = LoadTextureFromImage(image);
  SetTextureFilter(texture_, TEXTURE_FILTER_POINT);
  UnloadImage(image);
}

ChippyDisplay::~ChippyDisplay()
{
  UnloadTexture(texture_);
}

void ChippyDisplay::Draw(const std::array<std::uint32_t, INTERNAL_WIDTH * INTERNAL_HEIGHT> &buffer) const
{
  // Update texture on GPU
  UpdateTexture(texture_, &buffer);

  BeginDrawing();
  ClearBackground(BLACK);
  DrawTextureEx(texture_, {0, 0}, 0.0, WINDOW_WIDTH / INTERNAL_WIDTH, WHITE); // Scale up texture to window
  EndDrawing();
}