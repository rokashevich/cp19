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
    size_t instanced_num_bytes_;
    float* instanced_data_;
    int num_vertices_;
    int num_instances_;
  };
  std::unordered_map<std::type_index, Renderable*> renderables_;

 public:
  SDL_Event event;
  SDL_Window* window_;

  RendererSdl();
  void SetupStatic(std::type_index id, size_t num_bytes, const float* data,
                   const char* vertex_shader, const char* pixel_shader,
                   int num_vertices) {
    Renderable* renderable = new Renderable;
    renderable->shader = new Shader(vertex_shader, pixel_shader);
    renderable->num_vertices_ = num_vertices;
    glGenVertexArrays(1, &renderable->vao);
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindVertexArray(renderable->vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, num_bytes, data, GL_STATIC_DRAW);
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

  void UpdateDynamic(std::type_index id, size_t num_bytes, float* const data,
                    int num_instances) {
    Renderable* renderable = renderables_.find(id)->second;
    assert(renderable != nullptr);
    renderable->instanced_num_bytes_ = num_bytes;
    renderable->instanced_data_ = data;
    renderable->num_instances_ = num_instances;
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
      glBufferData(GL_ARRAY_BUFFER, renderable->instanced_num_bytes_,
                   renderable->instanced_data_, GL_DYNAMIC_DRAW);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                            (void*)0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glVertexAttribDivisor(1, 1);

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
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                            (void*)0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glVertexAttribDivisor(2, 0);
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
