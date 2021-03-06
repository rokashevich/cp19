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

class RendererSdl {
  float* projection_;
  float* view_;
  float* model_;
  struct Renderable {
    Shader* shader;
    unsigned int vao;

    int num_vertices_;
    int num_instances_;

    // Массивы float-ов для передачи в шейдеры.
    float* offsets_data_;  // Смещения относительно 0,0,0.
    size_t offsets_size_;

    float* angles_data_;  // Углы поворота вокруг соответствующих осей.
    size_t angles_size_;

    float* params_data_;  // Длина, ширина, здоровье базового объекта.
    size_t params_size_;
  };
  std::unordered_map<int, Renderable*> renderables_;

 public:
  SDL_Event event;
  SDL_Window* window_;

  RendererSdl();
  void SetupStatic(int id, const std::vector<float>* buffer,
                   const char* vertex_shader, const char* pixel_shader) {
    Renderable* renderable = new Renderable;
    renderable->shader = new Shader(vertex_shader, pixel_shader);
    renderable->num_vertices_ = buffer->size() / 3;
    glGenVertexArrays(1, &renderable->vao);
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindVertexArray(renderable->vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer->size(),
                 buffer->data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    renderables_[id] = renderable;
  }

  void UpdateCommon(float* projection, float* view, float* model) {
    projection_ = projection;
    view_ = view;
    model_ = model;
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void UpdateDynamic(int id, size_t offsets_size, float* offsets_data,
                     int num_instances, size_t angles_size, float* angles_data,
                     size_t params_size, float* params_data) {
    Renderable* renderable = renderables_.find(id)->second;

    renderable->num_instances_ = num_instances;

    renderable->offsets_size_ = offsets_size;
    renderable->offsets_data_ = offsets_data;

    renderable->angles_size_ = angles_size;
    renderable->angles_data_ = angles_data;

    renderable->params_size_ = params_size;
    renderable->params_data_ = params_data;
  }

  void RenderFrame() {
    for (auto& a : renderables_) {
      Renderable* renderable = a.second;
      renderable->shader->Use();
      glUniformMatrix4fv(
          glGetUniformLocation(renderable->shader->Program, "projection"), 1,
          GL_FALSE, projection_);
      glUniformMatrix4fv(
          glGetUniformLocation(renderable->shader->Program, "view"), 1,
          GL_FALSE, view_);
      glUniformMatrix4fv(
          glGetUniformLocation(renderable->shader->Program, "model"), 1,
          GL_FALSE, model_);
      glBindVertexArray(renderable->vao);

      unsigned int vbo;
      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, renderable->offsets_size_,
                   renderable->offsets_data_, GL_DYNAMIC_DRAW);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                            (void*)0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glVertexAttribDivisor(1, 1);

      unsigned int vbo1;
      glGenBuffers(1, &vbo1);
      glBindBuffer(GL_ARRAY_BUFFER, vbo1);
      glBufferData(GL_ARRAY_BUFFER, renderable->angles_size_,
                   renderable->angles_data_, GL_DYNAMIC_DRAW);
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
      for (auto i = 0; i < renderable->num_vertices_; ++i) {
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
      glVertexAttribDivisor(4, 0);

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
      glDrawArraysInstanced(GL_TRIANGLES, 0, renderable->num_vertices_,
                            renderable->num_instances_);
      glBindVertexArray(0);
    }
    SDL_GL_SwapWindow(window_);
  }

  ~RendererSdl() {
    for (auto& renderable : renderables_) delete renderable.second;
  }
};
