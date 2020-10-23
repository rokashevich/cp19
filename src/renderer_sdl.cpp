#include "renderer_sdl.hpp"

#include <iostream>

#include "constants.hpp"

RendererSdl::RendererSdl() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Could not initialize SDL" << std::endl;
    abort();
  }
  window_ = SDL_CreateWindow(nullptr, 0, 0, constants::screen_width,
                             constants::screen_height, SDL_WINDOW_OPENGL);
  if (!window_) {
    std::cout << "Could not initialize Window" << std::endl;
    abort();
  }
  SDL_GL_CreateContext(window_);
  SDL_SetRelativeMouseMode(SDL_TRUE);
}
