#include <EGL/egl.h>
#include <GLES3/gl32.h>
#include <time.h>

#include <array>
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

int main(int, char**) {  // С пустым main() падает на андроиде!
  enum { kWall, kMissile, kPlayer };

  World game_world = World(constants::maze_dimension);
  RendererSdl renderer;

  Object* reference_wall = new ObjectWall(0);
  Object* reference_missile = new ObjectMissile(P{0, 0, 0}, P{0, 0, 0}, 0);
  Object* reference_player = new ObjectPlayer(P{0, 0, 0}, P{0, 0, 0}, 0);

  Physics physics;
  physics.SetupObject(kWall, reference_wall, false);
  physics.SetupObject(kMissile, reference_missile, true);
  physics.SetupObject(kPlayer, reference_player, true);

  // Настраиваем стены.
  renderer.SetupStatic(kWall, reference_wall->SizeofVerticesBuffer(),
                       reference_wall->VerticesBuffer(), vertex_wall,
                       pixel_wall, reference_wall->NumVertices());
  // Настраиваем снаряды.
  renderer.SetupStatic(kMissile, reference_missile->SizeofVerticesBuffer(),
                       reference_missile->VerticesBuffer(), vertex_missile,
                       pixel_missile, reference_missile->NumVertices());
  // Настраиваем игрока.
  renderer.SetupStatic(kPlayer, reference_player->SizeofVerticesBuffer(),
                       reference_player->VerticesBuffer(), vertex_player,
                       pixel_player, reference_player->NumVertices());
  // Отладка.
  for (float i = 0; i < 10; ++i) {
    for (float j = 0; j < 10; ++j) {
      for (float k = 0; k < 10; ++k) {
        Object* o = new ObjectMissile(P{i, k, -j}, P{-1, 0, 1}, 0.1);
        physics.AddObject(kMissile, o);
      }
    }
  }

  Object* player1 = new ObjectPlayer(P{-5, 1, 1}, P{0, 1, -1});
  Object* player2 = new ObjectPlayer(P{0, 1, 1}, P{0, 1, 1});
  physics.AddObject(kPlayer, player1);
  physics.AddObject(kPlayer, player2);

  int done = 0;
  bool camera_toggle = true;

  while (!done) {
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

    // В первую очередь мир обновляет положения всех объектов на карте.
    physics.Step();

    renderer.UpdateCommon(&projection[0][0], &view[0][0], &model[0][0]);
    // Рисуем стены.
    renderer.UpdateDynamic(
        kWall,
        sizeof(float) * sizeof(float) * game_world.panels_data_array().size(),
        game_world.panels_data_array().data(), game_world.panels_count());
    // Рендерим снаряды.
    renderer.UpdateDynamic(kMissile, physics.SizeofCoordsParamsBuffer(kMissile),
                           physics.CoordsParamsBuffer(kMissile),
                           physics.ObjectsCount(kMissile));
    // Рендерим игроков.
    renderer.UpdateDynamic(kPlayer, physics.SizeofCoordsParamsBuffer(kPlayer),
                           physics.CoordsParamsBuffer(kPlayer),
                           physics.ObjectsCount(kPlayer));

    renderer.RenderFrame();
  }

  SDL_Quit();

  return 0;
}
