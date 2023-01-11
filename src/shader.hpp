#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

//#include <GL/glew.h>
#include <GLES3/gl32.h>

class Shader {
 public:
  GLuint Program;
  // Constructor generates the shader on the fly
  Shader(const char* vShaderCode, const char* fShaderCode, int key) {
    // 2. Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // Print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertex, 512, NULL, infoLog);
      SDL_Log("1 %d %s\n", key, infoLog);
    }
    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragment, 512, NULL, infoLog);
      SDL_Log("2 %d %s\n", key, infoLog);
    }
    // Shader Program
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    // Print linking errors if any
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
      SDL_Log("3 %d %s\n", key, infoLog);
    }
    // Delete the shaders as they're linked into our program now and no longer
    // necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }
  // Uses the current shader
  void Use() { glUseProgram(this->Program); }
};

#endif  // SHADER_H
