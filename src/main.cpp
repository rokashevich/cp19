#include <EGL/egl.h>
#include <GLES3/gl32.h>
#include <time.h>

#include <iostream>
#include <list>

// Сторонние библиотеки идут вместе с проектом.
#include "SDL.h"
#include "gameworld.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Сгенерированные из glsl файлов.
#include "shader_fragment_missile.hpp"
#include "shader_fragment_panel.hpp"
#include "shader_vertex_missile.hpp"
#include "shader_vertex_panel.hpp"

// Заголовочные файлы проекта
#include "object.hpp"
#include "physics.hpp"
#include "point.hpp"
#include "shader.hpp"

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700

// camera
static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

static float yaw =
    -90.0f;  // yaw is initialized to -90.0 degrees since a yaw of 0.0 results
             // in a direction vector pointing to the right so we initially
             // rotate a bit to the left.
static float pitch = 0.0f;
static float fov = 45.0f;

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

  // Настраиваем панели.
  const float d = 0.01;
  const float w = 0.45;
  const float panel_vertices[] = {
      -w, -w, -d, -w, -w, d,  w,  -w, -d,  // Нижняя крышка.
      w,  -w, d,  -w, -w, d,  w,  -w, -d,

      -w, w,  -d, -w, w,  d,  w,  w,  -d,  // Верхняя крышка.
      w,  w,  d,  -w, w,  d,  w,  w,  -d,

      -w, -w, d,  -w, w,  d,  w,  -w, d,  // Стенка xy.
      w,  w,  d,  -w, w,  d,  w,  -w, d,

      d,  -w, -d, d,  w,  -d, d,  -w, d,  // Стенка zy.
      d,  w,  d,  d,  w,  -d, d,  -w, d,

      -w, -w, -d, -w, w,  -d, w,  -w, -d,  // Стенка -xy.
      w,  w,  -d, -w, w,  -d, w,  -w, -d,

      -d, -w, -d, -d, w,  -d, -d, -w, d,  // Стенка -zy.
      -d, w,  d,  -d, w,  -d, -d, -w, d,
  };
  const int panel_vertices_count =
      sizeof(panel_vertices) / sizeof(*panel_vertices);
  unsigned int panel_VBO;
  glGenBuffers(1, &panel_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, panel_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(panel_vertices), panel_vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
  Shader panel_shader(shader_vertex_panel, shader_fragment_panel);

  // Настраиваем снаряды.
  const int missile_vertices_count = sizeof(O::vertices) / sizeof(*O::vertices);
  unsigned int missile_VBO;
  glGenBuffers(1, &missile_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, missile_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(O::vertices), O::vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), nullptr);
  glEnableVertexAttribArray(1);
  glBindVertexArray(1);
  Shader missile_shader(shader_vertex_missile, shader_fragment_missile);

  // Настраиваем игрока.
  const int player_vertices_count = sizeof(N::vertices) / sizeof(*N::vertices);
  unsigned int player_VBO;
  glGenBuffers(1, &player_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, player_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(O::vertices), O::vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), nullptr);
  glEnableVertexAttribArray(1);
  glBindVertexArray(1);
  Shader player_shader(shader_vertex_missile, shader_fragment_missile);

  // Отладка.
  O *o = new O(P{-1, 0, 0}, P{-1, 0, 1}, 2);
  Physics::AddO(o);
  Physics::AddO(new O(P{0, -1, 0}, P{0, -1, 1}, 2));
  // Physics::addO(new O(1, 3, 1, 1));
  // Physics::addO(new O(1, 4, 1, 1));
  // Physics::addO(new N(1, 1, 1, 1, 1, 1, 2, 1));
  // Physics::addO(new N(1, 1, 1, 1, 1, 1, 3, 1));

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  done = 0;
  int test = 0;  // Временная переменная не нужна.
  bool camera_toggle = true;

  while (!done) {
    // В первую очередь мир обновляет положения всех объектов на карте.
    Physics::Step();

    // Дальше делаем интерактив с объектами мира.
    while (SDL_PollEvent(&event)) {      
      if (event.type == SDL_QUIT) {
        done = 1;
      } else if (event.type == SDL_KEYDOWN) {
        // std::cout << "key down delta: " << Physics::Delta() << std::endl;
        const float cameraSpeed = 0.1f;
        switch (event.key.keysym.sym) {
          case SDLK_w:
            // std::cout << "w" << std::endl;
            cameraPos += cameraSpeed * cameraFront;
            break;
          case SDLK_s:
            // std::cout << "s" << std::endl;
            cameraPos -= cameraSpeed * cameraFront;
            break;
          case SDLK_a:
            // std::cout << "a" << std::endl;
            cameraPos -=
                glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            break;
          case SDLK_d:
            // std::cout << "d" << std::endl;
            cameraPos +=
                glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            break;
          case SDLK_t:
            std::cout << "toggle camera" << std::endl;
            camera_toggle = !camera_toggle;
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
    glm::mat4 view;
    if (camera_toggle)
      view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    else {
      glm::vec3 pos{o->x() - 2, o->y() - 2, o->z() - 2};
      view = glm::lookAt(pos, -pos - cameraFront, cameraUp);
    }
    // make sure to initialize matrix to identity matrix first
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    float angle = 0.0f;
    model =
        glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Рисуем стены.
    glUniformMatrix4fv(glGetUniformLocation(panel_shader.Program, "projection"),
                       1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(panel_shader.Program, "view"), 1,
                       GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(panel_shader.Program, "model"), 1,
                       GL_FALSE, &model[0][0]);
    const int panels_count = game_world.panels_count();
    std::vector<float> &panels_data_array = game_world.panels_data_array();
    if (test < 100) {
      //++test;
      unsigned int panels_VBO;
      glGenBuffers(1, &panels_VBO);
      glBindBuffer(GL_ARRAY_BUFFER, panels_VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * panels_data_array.size(),
                   panels_data_array.data(), GL_STATIC_DRAW);
      glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                            (void *)0);
      glVertexAttribDivisor(2, 1);
      glEnableVertexAttribArray(2);
      glBindVertexArray(2);
    }
    panel_shader.Use();
    glDrawArraysInstanced(GL_TRIANGLES, 0, panel_vertices_count, panels_count);

    // Рендерим снаряды.
    const Physics::RenderParameters rp = Physics::RenderParametersO();

    glUniformMatrix4fv(
        glGetUniformLocation(missile_shader.Program, "projection"), 1, GL_FALSE,
        &projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(missile_shader.Program, "view"), 1,
                       GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(missile_shader.Program, "model"), 1,
                       GL_FALSE, &model[0][0]);
    unsigned int missiles_VBO;
    glGenBuffers(1, &missiles_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, missiles_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rp.shader_data_size,
                 rp.shader_data, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)0);
    glVertexAttribDivisor(3, 1);
    glEnableVertexAttribArray(3);
    glBindVertexArray(3);
    missile_shader.Use();
    glDrawArraysInstanced(GL_TRIANGLES, 0, missile_vertices_count,
                          rp.objects_count);

    SDL_GL_SwapWindow(window);
  }

  SDL_Quit();

  return 0;
}
