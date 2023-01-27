#pragma once
#include <vector>

#include "object.hpp"
#include "shader.hpp"
struct Group {
  Group() = delete;
  Group(bool stationary, const char* vertex_shader_code,
        const char* pixel_shader_code, std::vector<float> vertices_buffer)
      : stationary{stationary},
        vertex_shader_code{vertex_shader_code},
        pixel_shader_code{pixel_shader_code},
        vertices_buffer{vertices_buffer},
        vertices_count{int(vertices_buffer.size() / 3)} {};
  bool stationary;
  const char* vertex_shader_code;
  const char* pixel_shader_code;
  std::vector<float> vertices_buffer;
  int vertices_count;
  std::vector<Object*> objects;
  std::vector<float> coords_buffer;
  std::vector<float> angles_buffer;
  std::vector<float> params_buffer;
  Shader* shader;
  unsigned int vao;
};