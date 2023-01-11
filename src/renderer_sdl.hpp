#pragma once
#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "SDL.h"
#include "shader.hpp"

class Renderable {
 public:
  Shader* shader;
  const float* coords_data_;
  const float* angles_data_;
  const float* params_data_;
  size_t coords_size_;
  size_t angles_size_;
  size_t params_size_;
  int num_indices_;    // вершин в шейпе
  int num_instances_;  // всего шейпов

  unsigned int vao;
  Renderable(const std::vector<float>* buffer, const char* vertex_shader,
             const char* pixel_shader, int key)
      : shader{new Shader{vertex_shader, pixel_shader, key}},
        num_indices_{int(buffer->size() / 3)} {
    glGenVertexArrays(1, &vao);
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer->size(),
                 buffer->data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // std::cout << ">>>" << num_shapes_ << " " << num_vertices_ << std::endl;
  }
};

class RendererSdl {
  float* projection_;
  float* view_;
  float* model_;

  std::unordered_map<int, Renderable*> renderables_;

 public:
  SDL_Event event;
  SDL_Window* window_;

  RendererSdl();
  void RegisterObject(int key, const std::vector<float>* buffer,
                      const char* vertex_shader, const char* pixel_shader);

  // Функция для передачи в кач-ве колбэка в физику. Физика же по мере
  // добавления/удаления объектов в игровой мир будет забрасывать с помощью этой
  // функции указатели на массивы для инстансированного рендеринга.
  void OnNumInstancesChanged(int key, int num_instances,
                             const float* coords_data, int coords_size,
                             const float* angles_data, int angles_size,
                             const float* params_data, int params_size);

  // Обновляет информацию по всем одинаково отрисовываемым шейпам.
  // void UpdateDynamic(int key, int num_shapes, const std::vector<float>&
  // coords,
  //                    const std::vector<float>& angles,
  //                    const std::vector<float>& params);

  // void UpdateDynamicOLD(int id, size_t coords_size, float* coords_data,
  //                       int num_instances, size_t angles_size,
  //                       float* angles_data, size_t params_size,
  //                       float* params_data) {
  //   Renderable* renderable = renderables_.find(id)->second;

  // renderable->num_shapes_ = num_instances;

  // renderable->coords_size_ = coords_size;
  // renderable->coords_data_ = coords_data;

  // renderable->angles_size_ = angles_size;
  // renderable->angles_data_ = angles_data;

  // renderable->params_size_ = params_size;
  // renderable->params_data_ = params_data;

  // отладочный вывод
  // if (id == 1) {
  //   for (int i = 0; i < coords_size; ++i) {
  //     std::cout << renderable->coords_data_[i] << " ";
  //   }
  //   std::cout << std::endl;
  // }
  // }

  void RenderFrame(float* projection, float* view, float* model);

  ~RendererSdl() {
    for (auto& renderable : renderables_) delete renderable.second;
  }
};
