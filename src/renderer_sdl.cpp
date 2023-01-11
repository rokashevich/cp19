#include "renderer_sdl.hpp"

#include <iostream>

#include "constants.hpp"

RendererSdl::RendererSdl() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
    std::cout << "SDL_Init: " << SDL_GetError() << std::endl;
    abort();
  }
  // SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  // SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  // SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
  // SDL_GL_CONTEXT_PROFILE_ES);
  /// window_ = SDL_CreateWindow(nullptr, 0, 0, constants::screen_width,
  ///                           constants::screen_height, SDL_WINDOW_OPENGL);
  // window_ = SDL_CreateWindow("My Game", SDL_WINDOWPOS_UNDEFINED,
  //                            SDL_WINDOWPOS_UNDEFINED, 0, 0,
  //                            SDL_WINDOW_FULLSCREEN_DESKTOP);
  /// SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);

  SDL_Rect gScreenRect = {0, 0, 320, 240};
  SDL_DisplayMode displayMode;
  if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0) {
    gScreenRect.w = displayMode.w;
    gScreenRect.h = displayMode.h;
  }
  window_ = SDL_CreateWindow(
      "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      gScreenRect.w, gScreenRect.h, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);

  if (!window_) {
    std::cout << "Could not initialize Window" << std::endl;
    abort();
  }
  SDL_GL_SetSwapInterval(1);
  SDL_GLContext context = SDL_GL_CreateContext(window_);
  SDL_GL_MakeCurrent(window_, context);
  // SDL_SetRelativeMouseMode(SDL_TRUE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}
void RendererSdl::RegisterObject(int key, const std::vector<float>* buffer,
                                 const char* vertex_shader,
                                 const char* pixel_shader) {
  renderables_[key] = new Renderable{buffer, vertex_shader, pixel_shader, key};
}
void RendererSdl::OnNumInstancesChanged(
    int key, int num_instances, const float* coords_data, int coords_size,
    const float* angles_data, int angles_size, const float* params_data,
    int params_size) {
  Renderable* r = renderables_[key];
  r->num_instances_ = num_instances;
  r->coords_data_ = coords_data;
  r->coords_size_ = coords_size * sizeof(float);
  // std::cout << num_instances << " " << coords_size << " " << r->coords_size_
  //           << std::endl;
  r->angles_data_ = angles_data;
  r->angles_size_ = angles_size * sizeof(float);
  r->params_data_ = params_data;
  r->params_size_ = params_size * sizeof(float);
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
void RendererSdl::RenderFrame(float* projection, float* view, float* model) {
  projection_ = projection;
  view_ = view;
  model_ = model;
  static float xxx = 1.0f;
  xxx -= 0.01f;
  if (xxx < 0.0f) xxx = 1.0f;
  glClearColor(1.0f, xxx, xxx, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (auto& a : renderables_) {
    Renderable* r = a.second;
    const auto key = a.first;

    r->shader->Use();
    glUniformMatrix4fv(glGetUniformLocation(r->shader->Program, "projection"),
                       1, GL_FALSE, projection_);
    glUniformMatrix4fv(glGetUniformLocation(r->shader->Program, "view"), 1,
                       GL_FALSE, view_);
    glUniformMatrix4fv(glGetUniformLocation(r->shader->Program, "model"), 1,
                       GL_FALSE, model_);
    glBindVertexArray(r->vao);
    SDL_Log("glBindVertexArray=%d\n", glGetError());

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, r->coords_size_, r->coords_data_,
                 GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(1, 1);

    unsigned int vbo1;
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, r->angles_size_, r->angles_data_,
                 GL_DYNAMIC_DRAW);
    // std::cout << renderable->num_instances_ << std::endl;
    // if (renderable->num_instances_ > 200)
    //   for (int i = 0; i < renderable->angles_size_; ++i) {
    //     std::cout << renderable->angles_data_[i];
    //   }
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);

    // Барицентрические координаты - что с  ними сделать?
    std::vector<float> bar;
    for (auto i = 0; i < r->num_indices_; ++i) {
      bar.push_back(1);
      bar.push_back(0);
      bar.push_back(0);

      bar.push_back(0);
      bar.push_back(1);
      bar.push_back(0);

      bar.push_back(0);
      bar.push_back(0);
      bar.push_back(1);
    }
    unsigned int instance_bar_VBO;
    glGenBuffers(1, &instance_bar_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, instance_bar_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * bar.size(), bar.data(),
                 GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    SDL_Log("glBindBuffer=%d\n", glGetError());
    glVertexAttribDivisor(4, 0);
    SDL_Log("glVertexAttribDivisor=%d\n", glGetError());

    // Далее отсылаем в шейдер координаты и поворты родителя (если есть).
    // std::vector<float> parent_offset{2, 2, 2};
    // unsigned int parent_offset_VBO;
    // glGenBuffers(1, &parent_offset_VBO);
    // glBindBuffer(GL_ARRAY_BUFFER, parent_offset_VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * parent_offset.size(),
    //              parent_offset.data(), GL_DYNAMIC_DRAW);
    // glEnableVertexAttribArray(3);
    // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    //                       (void*)0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glVertexAttribDivisor(3, 1);

    // Рисуем!
    // int key{a.first};
    // std::cout << key << " " << renderable->num_indices_ << " "
    //           << renderable->num_instances_ << std::endl;
    glDrawArraysInstanced(GL_TRIANGLES, 0, r->num_indices_, r->num_instances_);
    SDL_Log("glDrawArraysInstanced=%d\n", glGetError());
    // std::cout << r->num_indices_ << std::endl;
    // glBindVertexArray(0);
  }
  SDL_GL_SwapWindow(window_);
}