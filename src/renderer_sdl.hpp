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
             const char* pixel_shader)
      : shader{new Shader{vertex_shader, pixel_shader}},
        num_indices_{buffer->size() / 3} {
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
  void OnNumInstancesChanged(int key, int num_instances,
                             const std::vector<float>& coords,
                             const std::vector<float>& angles,
                             const std::vector<float>& params) {
    std::cout << "OnNumInstancesChanged:" << key << std::endl;
    Renderable* r = renderables_[key];

    r->num_instances_ = num_instances;

    r->coords_data_ = coords.data();
    r->coords_size_ = coords.size() * sizeof(float);

    r->angles_data_ = angles.data();
    r->angles_size_ = angles.size() * sizeof(float);

    r->params_data_ = params.data();
    r->params_size_ = params.size() * sizeof(float);
  }
  void UpdateCommon(float* projection, float* view, float* model) {
    projection_ = projection;
    view_ = view;
    model_ = model;
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

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

  void RenderFrame() {
    for (auto& a : renderables_) {
      Renderable* r = a.second;
      r->shader->Use();
      glUniformMatrix4fv(glGetUniformLocation(r->shader->Program, "projection"),
                         1, GL_FALSE, projection_);
      glUniformMatrix4fv(glGetUniformLocation(r->shader->Program, "view"), 1,
                         GL_FALSE, view_);
      glUniformMatrix4fv(glGetUniformLocation(r->shader->Program, "model"), 1,
                         GL_FALSE, model_);
      glBindVertexArray(r->vao);

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
      // int key{a.first};
      // std::cout << key << " " << renderable->num_indices_ << " "
      //           << renderable->num_instances_ << std::endl;
      glDrawArraysInstanced(GL_TRIANGLES, 0, r->num_indices_,
                            r->num_instances_ * 2);
      glBindVertexArray(0);
    }
    SDL_GL_SwapWindow(window_);
  }

  ~RendererSdl() {
    for (auto& renderable : renderables_) delete renderable.second;
  }
};
