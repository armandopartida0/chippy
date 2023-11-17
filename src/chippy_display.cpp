#include "chippy_display.hpp"

ChippyDisplay::ChippyDisplay()
{
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "chippy");

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

void ChippyDisplay::Draw(std::array<uint32_t, 64 * 32> buffer)
{
  // Update texture on GPU
  UpdateTexture(texture_, &buffer);

  BeginDrawing();
  ClearBackground(BLACK);
  DrawTextureEx(texture_, {0, 0}, 0.0, 20.0, WHITE);
  EndDrawing();
}