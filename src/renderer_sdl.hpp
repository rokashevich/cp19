#pragma once
#include <array>
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
  std::vector<Group>* groups_;
  float* projection_;
  float* view_;
  float* model_;

 public:
  SDL_Event event;
  SDL_Window* window_;

  RendererSdl(std::vector<Group>* groups) : groups_{groups} {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_SENSOR) < 0) {
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
    window_ =
        SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, gScreenRect.w, gScreenRect.h,
                         SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);

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

    // Инициаллизируем шейдеры групп.
    for (auto& group : *groups_) {
      group.shader =
          new Shader{group.vertex_shader_code, group.pixel_shader_code};
      glGenVertexArrays(1, &group.vao);
      unsigned int vbo;
      glGenBuffers(1, &vbo);
      glBindVertexArray(group.vao);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER,
                   sizeof(float) * group.vertices_buffer.size(),
                   group.vertices_buffer.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float),
                            nullptr);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }
  }

  void RenderFrame(float* projection, float* view, float* model) {
    projection_ = projection;
    view_ = view;
    model_ = model;
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  //цвет мира
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& group : *groups_) {
      group.shader->Use();
      glUniformMatrix4fv(
          glGetUniformLocation(group.shader->Program, "projection"), 1,
          GL_FALSE, projection_);
      glUniformMatrix4fv(glGetUniformLocation(group.shader->Program, "view"), 1,
                         GL_FALSE, view_);
      glUniformMatrix4fv(glGetUniformLocation(group.shader->Program, "model"),
                         1, GL_FALSE, model_);
      glBindVertexArray(group.vao);

      unsigned int vbo;
      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, group.coords_buffer.size() * sizeof(float),
                   group.coords_buffer.data(), GL_DYNAMIC_DRAW);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                            (void*)0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glVertexAttribDivisor(1, 1);

      unsigned int vbo1;
      glGenBuffers(1, &vbo1);
      glBindBuffer(GL_ARRAY_BUFFER, vbo1);
      glBufferData(GL_ARRAY_BUFFER, group.angles_buffer.size() * sizeof(float),
                   group.angles_buffer.data(), GL_DYNAMIC_DRAW);
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                            (void*)0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glVertexAttribDivisor(2, 1);

      // Барицентрические координаты - что с  ними сделать?
      std::vector<float> bar;
      for (auto i = 0; i < group.vertices_count; ++i) {
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

      // count - вершин(точек) в базовой фигуре
      // instancecount - всего фигур
      // std::cout << "group.objects.size()=" << group.objects.size();
      // std::cout << " group.coords_buffer.size()=" <<
      // group.coords_buffer.size(); std::cout << " group.vertices_count=" <<
      // group.vertices_count; std::cout << std::endl;
      glDrawArraysInstanced(GL_TRIANGLES, 0, group.vertices_count,
                            group.objects.size());
    }
    SDL_GL_SwapWindow(window_);
  }

  ~RendererSdl() {}
};
