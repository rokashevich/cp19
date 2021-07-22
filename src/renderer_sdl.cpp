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
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}
void RendererSdl::RegisterObject(int key, const std::vector<float>* buffer,
                                 const char* vertex_shader,
                                 const char* pixel_shader) {
  renderables_[key] = new Renderable{buffer, vertex_shader, pixel_shader};
}
// void RendererSdl::UpdateDynamic(int key, int num_shapes,
//                                 const std::vector<float>& coords,
//                                 const std::vector<float>& angles,
//                                 const std::vector<float>& params) {
//   Renderable* renderable = renderables_.find(key)->second;
// renderable->num_shapes_ = num_shapes;
// renderable->coords_size_ = coords.size();
// renderable->coords_data_ = coords.data();

// renderable->angles_size_ = angles.size();
// renderable->angles_data_ = angles.data();

// renderable->params_size_ = params.size();
// renderable->params_data_ = params.data();

// if (id == 1) {
//   for (int i = 0; i < coords_size; ++i) {
//     std::cout << renderable->coords_data_[i] << " ";
//   }
//   std::cout << std::endl;
// }
// }