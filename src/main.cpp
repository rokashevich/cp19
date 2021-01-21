#include <EGL/egl.h>
#include <GLES3/gl32.h>
#include <time.h>

#include <array>
#include <iostream>
#include <list>
#include <typeinfo>
#include <unordered_map>
// Сторонние библиотеки идут вместе с проектом.
#include "SDL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "world.hpp"

// Сгенерированные из glsl файлов.
#include "pixel_gun.hpp"
#include "pixel_missile.hpp"
#include "pixel_player.hpp"
#include "pixel_wall.hpp"
#include "vertex_gun.hpp"
#include "vertex_missile.hpp"
#include "vertex_player.hpp"
#include "vertex_wall.hpp"

// Заголовочные файлы проекта

#include "constants.hpp"
#include "generator_shape.hpp"
#include "object.hpp"
#include "object_gun.hpp"
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

struct Cfg {
  Object* reference;
  const bool is_dynamic;
  const char* vertex_shader;
  const char* pixel_shader;
};

// auto& a = Shape<ObjectWall>::ShapeVerticesBuffer2();

int main(int, char**) {  // С пустым main() падает на андроиде!
  // Каждому типу объектов - уникальный key.
  enum { wall, missile, player, gun };
  std::unordered_map<int, ObjectsStaticInfo> cfgs2{
      {wall, Shape<ObjectWall>::StaticInfo()},
      {missile, Shape<ObjectMissile>::StaticInfo()},
      {player, Shape<ObjectPlayer>::StaticInfo()},
      {gun, Shape<ObjectGun>::StaticInfo()},
  };
  std::unordered_map<int, Cfg> cfgs{
      {wall, {new ObjectWall(), false, vertex_wall, pixel_wall}},
      {missile, {new ObjectMissile(), true, vertex_missile, pixel_missile}},
      {player, {new ObjectPlayer(), true, vertex_player, pixel_player}},
      {gun, {new ObjectGun(), true, vertex_gun, pixel_gun}}};
  //                          ^^^ static или dynaimic

  World game_world = World(constants::maze_dimension);
  RendererSdl renderer;
  Physics physics;
  for (auto const& [key, cfg] : cfgs) {
    physics.SetupObject(key, cfg.reference, cfgs2[key]);
  }
  for (auto const& [key, cfg] : cfgs2) {
    renderer.SetupStatic(key, &cfg.vertices_buffer, cfg.vertex_shader,
                         cfg.pixel_shader);
  }

  // Отладка.
  for (unsigned long i = 0; i < game_world.panels_data_array().size();) {
    float x = game_world.panels_data_array().at(i++);
    float y = game_world.panels_data_array().at(i++);
    float z = game_world.panels_data_array().at(i++);
    float w = game_world.panels_data_array().at(i++);
    Object* o = new ObjectWall(Vec(x, y, z), w);
    physics.AddObject(wall, o);
  }
  for (float i = 0; i < 10; ++i) {
    for (float j = 0; j < 10; ++j) {
      for (float k = 0; k < 10; ++k) {
        Object* o = new ObjectMissile(Vec(i * 3, k * 3, -j * 3, -1, 0, 1), 0.5);
        physics.AddObject(missile, o);
      }
    }
  }
  Object* player2 = new ObjectPlayer(Vec(-5, 1, 1, 0, 1, -1));
  Object* player1 = new ObjectPlayer(Vec(0, -0.45, 1, 0, 1, 1));
  physics.AddObject(player, player2);
  physics.AddObject(player, player1);

  Object* gun1 = new ObjectGun(Vec());
  gun1->Owner(player1);
  physics.AddObject(gun, gun1);

  int done = 0;
  bool camera_toggle = true;
  bool right_mouse_button_held = false;
  while (!done) {
    // Дальше делаем интерактив с объектами мира.
    while (SDL_PollEvent(&renderer.event)) {
      if (renderer.event.type == SDL_QUIT) {
        done = 1;
      } else if (renderer.event.type == SDL_MOUSEBUTTONDOWN ||
                 renderer.event.type == SDL_MOUSEBUTTONUP) {
        if (renderer.event.button.button == SDL_BUTTON_LEFT) {
          std::cout << "LMB" << std::endl;
        } else if (renderer.event.button.button == SDL_BUTTON_RIGHT) {
          right_mouse_button_held =
              renderer.event.button.state == SDL_PRESSED ? true : false;
        }
      } else if (renderer.event.type == SDL_KEYDOWN) {
        std::cout << "key down delta: " << renderer.event.key.keysym.sym
                  << std::endl;
        const float cameraSpeed = 0.2f;
        switch (renderer.event.key.keysym.sym) {
          case SDLK_w:
            //            std::cout << "w" << std::endl;
            cameraPos += cameraSpeed * cameraFront;
            break;
          case SDLK_s:
            //            std::cout << "s" << std::endl;
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

    physics.Step();
    renderer.UpdateCommon(&projection[0][0], &view[0][0], &model[0][0]);
    for (auto const& [key, cfg] : cfgs)
      renderer.UpdateDynamic(key, physics.SizeofCoordsParamsBuffer(key),
                             physics.CoordsParamsBuffer(key),
                             physics.NumShapes(key));

    renderer.RenderFrame();
  }

  SDL_Quit();

  return 0;
}
