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
static glm::vec3 cameraPos =
    glm::vec3(0.0f, 0.0f, .0f);  // отладка: позиция примерно сверху лабиринта
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
static float yaw = 0.0f;
static float pitch = 0.0f;

struct Cfg {
  Object *reference;
  const bool is_dynamic;
  const char *vertex_shader;
  const char *pixel_shader;
};

// auto& a = Shape<ObjectWall>::ShapeVerticesBuffer2();

int main(int, char **) {  // С пустым main() падает на андроиде!
  // Каждому типу объектов - уникальный key.
  enum { wall, missile, player };
  std::unordered_map<int, ObjectsStaticInfo> cfgs2{
      {wall, Shape<ObjectWall>::StaticInfo()},
      {missile, Shape<ObjectMissile>::StaticInfo()},
      {player, Shape<ObjectPlayer>::StaticInfo()}};
  std::unordered_map<int, Cfg> cfgs{
      {wall, {new ObjectWall(), false, vertex_wall, pixel_wall}},
      {missile, {new ObjectMissile(), true, vertex_missile, pixel_missile}},
      {player, {new ObjectPlayer(), true, vertex_player, pixel_player}}};

  World game_world = World(constants::maze_dimension);
  RendererSdl renderer;
  Physics physics;
  for (auto const &[key, cfg] : cfgs) {
    physics.SetupObject(key, cfg.reference, cfgs2[key]);
  }
  for (auto const &[key, cfg] : cfgs2) {
    renderer.SetupStatic(key, &cfg.vertices_buffer, cfg.vertex_shader,
                         cfg.pixel_shader);
  }

  // Отладка.
  for (unsigned long i = 0; i < game_world.panels_data_array().size();) {
    float x = game_world.panels_data_array().at(i++);
    float y = game_world.panels_data_array().at(i++);
    float z = game_world.panels_data_array().at(i++);
    float w = game_world.panels_data_array().at(i++);
    std::cout << x << " " << y << " " << z << std::endl;
    Object *o = new ObjectWall(Vec(x, y, z, x, y, z), w);
    physics.AddObject(wall, o);
  }
  for (float i = 0; i < 5; ++i) {
    for (float j = 0; j < 5; ++j) {
      for (float k = 0; k < 5; ++k) {
        Object *o = new ObjectMissile(
            Vec(0 + i, 20 + j, 5 + k, 0 + i, 19 + j, 5 + k), 0.5);
        physics.AddObject(missile, o);
      }
    }
  }
  //  Object* o = new ObjectMissile(Vec(0, 20, 5, 0, 21, 5), 10);
  //  physics.AddObject(missile, o);
  // Object* player2 = new ObjectPlayer(Vec(-5, 1, 1, 0, 1, -1));
  Object *player1 = new ObjectPlayer(Vec(0, 0, 0, 0, 14, 5));
  // physics.AddObject(player, player2);
  physics.AddObject(player, player1);

  // Основной игровой цикл!
  int done = 0;
  bool camera_toggle = true;
  const float cameraSpeed = 0.2f;
  while (!done) {
    // Дальше делаем интерактив с объектами мира.
    while (SDL_PollEvent(&renderer.event)) {
      if (renderer.event.type == SDL_QUIT) {
        done = 1;
      } else if (renderer.event.type == SDL_KEYDOWN) {
        switch (renderer.event.key.keysym.sym) {
          case SDLK_w:
            cameraPos += cameraSpeed * cameraFront;
            break;
          case SDLK_s:
            cameraPos -= cameraSpeed * cameraFront;
            break;
          case SDLK_a:
            cameraPos -=
                glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            break;
          case SDLK_d:
            cameraPos +=
                glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            break;
          case SDLK_t:
            camera_toggle = !camera_toggle;
            break;
          default:
            break;
        }
      } else if (renderer.event.type == SDL_MOUSEMOTION && !camera_toggle) {
        static float sensitivity = 0.1f;
        yaw += renderer.event.motion.xrel * sensitivity;
        pitch -= renderer.event.motion.yrel * sensitivity;
        // when pitch is out of bounds
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
      }
    }

    // Расчитываем перемещения всех объектов с учётом введённых в предыдущем
    // блоке интеракций.
    physics.Step();

    // Что-то с матрицами...
    // pass projection matrix to shader (note that in this case it could change
    // every frame)
    glm::mat4 projection =
        glm::perspective(glm::radians(100.0f),
                         static_cast<float>(constants::screen_width) /
                             static_cast<float>(constants::screen_height),
                         1.0f, 100.0f);
    // camera/view transformation
    glm::mat4 view;
    if (!camera_toggle)
      view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    else {
      glm::vec3 followed{player1->V().Begin().x, player1->V().Begin().y,
                         player1->V().Begin().z};
      view = glm::lookAt(followed + glm::vec3{-2, 2, -2}, followed,
                         glm::vec3{0, 1, 0});
    }
    // make sure to initialize matrix to identity matrix first
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    float angle = 10.0f;
    model =
        glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    renderer.UpdateCommon(&projection[0][0], &view[0][0], &model[0][0]);
    for (auto const &[key, cfg] : cfgs)
      renderer.UpdateDynamic(key, physics.SizeofCoordsParamsBuffer(key),
                             physics.CoordsParamsBuffer(key),
                             physics.NumShapes(key));
    renderer.RenderFrame();
  }

  SDL_Quit();

  return 0;
}
