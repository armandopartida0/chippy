#include "chippy_display.h"

ChippyDisplay::ChippyDisplay()
{
  /* Setup SDL2 window */
  if(SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cout << "Failed to initialize SDL Video subsystem \n";
  }
  else
  {
    window_ = SDL_CreateWindow("chippy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
      kwindow_width_, kwindow_height_, SDL_WINDOW_SHOWN);
		renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
  }
}

ChippyDisplay::~ChippyDisplay()
{
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

void ChippyDisplay::Draw(std::array<uint32_t, 64 * 32> buffer)
{
  SDL_Texture* texture = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, 
    SDL_TEXTUREACCESS_STREAMING, 64, 32);

  SDL_UpdateTexture(texture, nullptr, buffer.data(), sizeof(buffer[0]) * 64);
	SDL_RenderClear(renderer_);
	SDL_RenderCopy(renderer_, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer_);
}