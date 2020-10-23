#include <EGL/egl.h>
#include <GLES3/gl32.h>
#include <time.h>

#include <iostream>
#include <list>
#include <typeinfo>
// Сторонние библиотеки идут вместе с проектом.
#include "SDL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "world.hpp"

// Сгенерированные из glsl файлов.
#include "pixel_missile.hpp"
#include "pixel_player.hpp"
#include "pixel_wall.hpp"
#include "vertex_missile.hpp"
#include "vertex_player.hpp"
#include "vertex_wall.hpp"

// Заголовочные файлы проекта
#include "constants.hpp"
#include "generator_shape.hpp"
#include "object.hpp"
#include "object_missile.hpp"
#include "object_player.hpp"
#include "object_wall.hpp"
#include "physics.hpp"
#include "point.hpp"
#include "renderer_sdl.hpp"
#include "shader.hpp"
#include "shape.hpp"

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

struct Square {
  float x;
  float y;
  float z;
  float a;
  float t;
};

int main(int, char **) {  // С пустым main() падает на андроиде!
  World game_world = World(constants::maze_dimension);
  RendererSdl renderer;
  // Настраиваем снаряды.
  renderer.SetupStatic(typeid(ObjectMissile), Shape<ObjectMissile>::NumBytes(),
                       Shape<ObjectMissile>::Data(), vertex_missile,
                       pixel_missile, Shape<ObjectMissile>::NumVertices());
  // Настраиваем игрока.
  renderer.SetupStatic(typeid(ObjectPlayer), Shape<ObjectPlayer>::NumBytes(),
                       Shape<ObjectPlayer>::Data(), vertex_player, pixel_player,
                       Shape<ObjectPlayer>::NumVertices());
  // Настраиваем панели.
  renderer.SetupStatic(typeid(ObjectWall), Shape<ObjectWall>::NumBytes(),
                       Shape<ObjectWall>::Data(), vertex_wall, pixel_wall,
                       Shape<ObjectWall>::NumVertices());
  // Отладка.
  for (float i = 0; i < 10; ++i) {
    for (float j = 0; j < 10; ++j) {
      for (float k = 0; k < 10; ++k) {
        Physics::AddO(new ObjectMissile(P{i, k, -j}, P{-1, 0, 1}, 0.5, 1));
      }
    }
  }

  Physics::AddN(new ObjectPlayer(P{-5, 1, 1}, P{0, 1, -1}, 2));
  Physics::AddN(new ObjectPlayer(P{0, 1, 1}, P{0, 1, 1}, 2));

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  int done = 0;
  bool camera_toggle = true;

  while (!done) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // В первую очередь мир обновляет положения всех объектов на карте.
    Physics::Step();

    // Дальше делаем интерактив с объектами мира.
    while (SDL_PollEvent(&renderer.event)) {
      if (renderer.event.type == SDL_QUIT) {
        done = 1;
      } else if (renderer.event.type == SDL_KEYDOWN) {
        // std::cout << "key down delta: " << Physics::Delta() << std::endl;
        const float cameraSpeed = 0.2f;
        switch (renderer.event.key.keysym.sym) {
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
      } else if (renderer.event.type == SDL_MOUSEMOTION) {
        static float sensitivity = 0.1f;  // change this value to your liking
        yaw += renderer.event.motion.xrel * sensitivity;
        pitch -= renderer.event.motion.yrel * sensitivity;

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
    glm::mat4 projection =
        glm::perspective(glm::radians(fov),
                         static_cast<float>(constants::screen_width) /
                             static_cast<float>(constants::screen_height),
                         0.1f, 100.0f);
    // camera/view transformation
    glm::mat4 view;
    // if (camera_toggle)
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    //    else {
    //      glm::vec3 pos{o->x() - 2, o->y() - 2, o->z() - 2};
    //      view = glm::lookAt(pos, -pos - cameraFront, cameraUp);
    //    }
    // make sure to initialize matrix to identity matrix first
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    float angle = 0.0f;
    model =
        glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

    renderer.SetupDynamicCommon(&projection[0][0], &view[0][0], &model[0][0]);
    // Рендерим снаряды.
    const Physics::RenderParameters rp_o = Physics::RenderParametersO();
    renderer.SetupDynamic(typeid(ObjectMissile),
                          sizeof(float) * rp_o.shader_data_size,
                          rp_o.shader_data, rp_o.objects_count);
    // Рисуем стены.
    renderer.SetupDynamic(
        typeid(ObjectWall),
        sizeof(float) * sizeof(float) * game_world.panels_data_array().size(),
        game_world.panels_data_array().data(), game_world.panels_count());
    // Рендерим игроков.
    const Physics::RenderParameters rp_n = Physics::RenderParametersN();
    renderer.SetupDynamic(typeid(ObjectPlayer),
                          sizeof(float) * rp_n.shader_data_size,
                          rp_n.shader_data, rp_n.objects_count);

    renderer.RenderFrame();
  }

  SDL_Quit();

  return 0;
}
