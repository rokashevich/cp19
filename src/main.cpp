#include <EGL/egl.h>
#include <GLES3/gl32.h>
#include <time.h>
#include <iostream>

// Сторонние библиотеки идут вместе с проектом.
#include "SDL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Сгенерированные из glsl файлов.
#include "shader_fragment_panel.hpp"
#include "shader_vertex_panel.hpp"
#include "shader_vertex_rib.hpp"

#include "gameworld.hpp"
#include "shader.hpp"

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700

// camera
static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

static float yaw =
    -90.0f;  // yaw is initialized to -90.0 degrees since a yaw of 0.0 results
             // in a direction vector pointing to the right so we initially
             // rotate a bit to the left.
static float pitch = 0.0f;
static float fov = 45.0f;

// timing
static unsigned int deltaTicks = 0;
static unsigned int lastTicks = 0;

void render() {
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glClearColor(1.f, 0.f, 1.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT);
}

struct Square {
  float x;
  float y;
  float z;
  float a;
  float t;
};

int main(int, char **) {  // С пустым main() падает на андроиде!

  GameWorld game_world = GameWorld(5);  // TODO размер мира вынести в конфиг.

  std::cout << game_world;

  SDL_Window *window;
  int done;
  SDL_Event event;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Could not initialize SDL\n");
    return 1;
  }
  window = SDL_CreateWindow(nullptr, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_OPENGL);
  if (!window) {
    printf("Could not initialize Window\n");
    return 1;
  }
  SDL_GL_CreateContext(window);
  SDL_SetRelativeMouseMode(SDL_TRUE);

  Shader panel_shader(shader_vertex_panel, shader_fragment_panel);
  Shader rib_shader(shader_vertex_rib, shader_fragment_panel);

  float panel_vertices[] = {-0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f,
                            -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,  0.5f};
  float rib_vertices[] = {-0.5, 0.0, 0.5, 0.0};

  // Константные значения для шейдера панелей.
  panel_shader.Use();
  glUniform1iv(glGetUniformLocation(panel_shader.Program, "panels_permanent"),
               GameWorld::kPanelsPermanentParamsCount,
               game_world.panels_permanent_parameters());
  unsigned int panel_VBO;
  glGenBuffers(1, &panel_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, panel_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(panel_vertices), panel_vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  // Константные значенияn для шейдера рёбер.
  rib_shader.Use();
  unsigned int rib_VBO;
  glGenBuffers(1, &rib_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, rib_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rib_vertices), rib_vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0 * sizeof(float), nullptr);

  glEnableVertexAttribArray(0);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  done = 0;
  while (!done) {
    unsigned int currentTicks = SDL_GetTicks();
    deltaTicks = currentTicks - lastTicks;
    lastTicks = currentTicks;
    if (deltaTicks == 0) deltaTicks = 1;

    while (SDL_PollEvent(&event)) {
      // std::cout << deltaTicks << std::endl;
      if (event.type == SDL_QUIT) {
        done = 1;
      } else if (event.type == SDL_KEYDOWN) {
        std::cout << "key down" << std::endl;
        float cameraSpeed = 0.05f * deltaTicks;
        switch (event.key.keysym.sym) {
          case SDLK_w:
            std::cout << "w" << std::endl;
            cameraPos += cameraSpeed * cameraFront;
            break;
          case SDLK_s:
            std::cout << "s" << std::endl;
            cameraPos -= cameraSpeed * cameraFront;
            break;
          case SDLK_a:
            std::cout << "a" << std::endl;
            cameraPos -=
                glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            break;
          case SDLK_d:
            std::cout << "d" << std::endl;
            cameraPos +=
                glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            break;
          default:
            break;
        }
      } else if (event.type == SDL_MOUSEMOTION) {
        static float sensitivity = 0.1f;  // change this value to your liking
        yaw += event.motion.xrel * sensitivity;
        pitch -= event.motion.yrel * sensitivity;

        // make sure that when pitch is out of bounds, screen doesn't get
        // flipped
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
      }
    }

    // pass projection matrix to shader (note that in this case it could change
    // every frame)
    glm::mat4 projection = glm::perspective(
        glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f,
        100.0f);
    // camera/view transformation
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    // make sure to initialize matrix to identity matrix first
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    float angle = 0.0f;
    model =
        glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    panel_shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(panel_shader.Program, "projection"),
                       1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(panel_shader.Program, "view"), 1,
                       GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(panel_shader.Program, "model"), 1,
                       GL_FALSE, &model[0][0]);

    // Отправляем в шейдер инстансированный массив панелей.
    const float *panels_array = game_world.panels_instanced_array();
    const int panels_count = game_world.panels_instanced_size();
    unsigned int panels_VBO;
    glGenBuffers(1, &panels_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, panels_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * panels_count, panels_array,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void *)0);
    glVertexAttribDivisor(1, 1);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, panels_count);

    // Отправляем в шейдер инстансированный массив рёбер.
    const float *ribs_array = game_world.ribs_instanced_array();
    const int ribs_count = game_world.ribs_instanced_size();
    unsigned int instanceVBO_ribs;
    glGenBuffers(1, &instanceVBO_ribs);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO_ribs);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ribs_count, ribs_array,
                 GL_STATIC_DRAW);

    SDL_GL_SwapWindow(window);
  }

  SDL_Quit();

  return 0;
}
