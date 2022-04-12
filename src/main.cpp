#include <EGL/egl.h>
#include <GLES3/gl32.h>
#include <bits/stdint-intn.h>
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

// Сгенерированные из glsl файлов.
#include "pixel_missile.hpp"
#include "pixel_player.hpp"
#include "pixel_wall.hpp"
#include "vertex_missile.hpp"
#include "vertex_player.hpp"
#include "vertex_wall.hpp"

// Заголовочные файлы проекта
#include "../net/interplay.hpp"
#include "constants.hpp"
#include "generator_shape.hpp"
#include "missile.hpp"
#include "object.hpp"
#include "physics.hpp"
#include "player.hpp"
// #include "point.hpp"
#include "renderer_sdl.hpp"
#include "shader.hpp"
#include "t.hpp"
#include "wall.hpp"
#include "world.hpp"

// camera
static glm::vec3 cameraPos =
    glm::vec3(0.0f, 0.0f, .0f);  // отладка: позиция примерно сверху лабиринта
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
static float yaw = 0.0f;
static float pitch = 0.0f;

struct Cfg {
  ShapeInfo static_info;
  const bool is_dynamic;
  const char *vertex_shader;
  const char *pixel_shader;
};

// auto& a = Shape<ObjectWall>::ShapeVerticesBuffer2();

int main(int, char **) {  // С пустым main() падает на андроиде!
  Interplay::SingletonClient::Instance().SendMessage("I'm from main!\n");

  // Все возможные типы объектов:
  enum { wall, player, weapon, missile, collectable, graffity };

  std::unordered_map<int, Cfg> cfgs{
      {wall, {Shape<Wall>::StaticInfo(), false, vertex_wall, pixel_wall}},
      {player,
       {Shape<Player>::StaticInfo(), true, vertex_player, pixel_player}}};
  /*,
        {missile,
         {Shape<Missile>::StaticInfo(), new Missile(), true, vertex_missile,
          pixel_missile}}*/
  World game_world = World(constants::maze_dimension);
  RendererSdl renderer;

  // Создаём физический мир и регистрируем в нём колбэк, которым в рендерер
  // будут отправляться изменённые статически данные: указатели на массивы с
  // координатами, углами и параметрами, которые рендерер будет рисовать.
  Physics physics([&](int key, int num_instances, const float *coords_data,
                      int coords_size, const float *angles_data,
                      int angles_size, const float *params_data,
                      int params_size) {
    renderer.OnNumInstancesChanged(key, num_instances, coords_data, coords_size,
                                   angles_data, angles_size, params_data,
                                   params_size);
  });

  for (auto iter : cfgs) {
    auto key = iter.first;
    auto cfg = iter.second;
    physics.RegisterObject(key);
    renderer.RegisterObject(key, &cfg.static_info.vertices_buffer,
                            cfg.vertex_shader, cfg.pixel_shader);
  }

  // Отладка.
  for (unsigned long i = 0; i < game_world.panels_data_array().size();) {
    float x = game_world.panels_data_array().at(i++);
    float y = game_world.panels_data_array().at(i++);
    float z = game_world.panels_data_array().at(i++);
    float a = game_world.panels_data_array().at(i++);
    float b = game_world.panels_data_array().at(i++);
    float c = game_world.panels_data_array().at(i++);
    float width = game_world.panels_data_array().at(i++);
    float height = game_world.panels_data_array().at(i++);
    float health = game_world.panels_data_array().at(i++);
    glm::vec3 angles = {a, b, c};
    glm::vec4 params = {width, height, health, 0.0f};
    // std::cout << a << "-" << b << "-" << z << std::endl;
    Object *o = new Wall(glm::vec3{x, y, z}, angles, params);
    physics.AddObject(wall, o);
  }
  // Тестовые снаряды.
  // for (float i = 0; i < 5; ++i) {
  //   for (float j = 0; j < 5; ++j) {
  //     for (float k = 0; k < 5; ++k) {
  //       Object *o =
  //           new Missile(V(0 + i, 20 + j, 5 + k, 0 + i, 19 + j, 5 + k),
  //           0.5);
  //       physics.AddObject(missile, o);
  //     }
  //   }
  // }
  // Object *o = new Missile(V(0, 20, 5, 0, 19, 5), 0.5);l
  // physics.AddObject(missile, o);

  // Тестовые игроки.
  //  Object* o = new ObjectMissile(V(0, 20, 5, 0, 21, 5), 10);
  //  physics.AddObject(missile, o);
  // Object* player2 = new ObjectPlayer(V(-5, 1, 1, 0, 1, -1));

  auto const coords = glm::vec3{0, 0, 0};
  auto const angles = glm::vec3{0, 0, 0};
  auto const head{1};
  auto const body{1};
  auto const arms{1};
  auto const legs{1};
  Object *player1 = new Player(coords, angles, head, body, arms, legs);

  // physics.AddObject(player, player2);
  physics.AddObject(player, player1);

  // Основной игровой цикл!
  int done = 0;
  bool input_bound_to_object = true;
  const float cameraSpeed = 0.2f;
  Object *controlled = player1;

  while (!done) {
    tribool backward_forward, left_right;
    static bool up;

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&renderer.event)) {
      if (renderer.event.type == SDL_QUIT) {
        done = 1;
      } else if (renderer.event.type == SDL_KEYDOWN ||
                 renderer.event.type == SDL_KEYUP) {
        if (state[SDL_SCANCODE_W]) ++backward_forward;
        if (state[SDL_SCANCODE_S]) --backward_forward;
        if (state[SDL_SCANCODE_A]) --left_right;
        if (state[SDL_SCANCODE_D]) ++left_right;
        if (state[SDL_SCANCODE_SPACE]) up = !up;
        if (state[SDL_SCANCODE_T] && renderer.event.type != SDL_KEYUP)
          input_bound_to_object = !input_bound_to_object;
      } else if (renderer.event.type == SDL_MOUSEMOTION &&
                 !input_bound_to_object) {
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

    // Преобразуем запрошенное управление из формата кнопок
    // вперёд-назад/влево-вправо/вверх в вектор в координатах игрового мира
    glm::mat4 view;
    // glm::mat4 trans = glm::mat4(1.0f);
    if (input_bound_to_object) {  // Управление объектом.

      controlled->Move(backward_forward, left_right, tribool{up});

      glm::vec3 followed{player1->v().Begin().x(), player1->v().Begin().y(),
                         player1->v().Begin().z()};
      view = glm::lookAt(followed + glm::vec3{-2, 2, 0}, followed,
                         glm::vec3{0, 1, 0});
    } else {  // Управление камерой.
      if (backward_forward > 0) cameraPos += cameraSpeed * cameraFront;
      if (backward_forward < 0) cameraPos -= cameraSpeed * cameraFront;
      if (left_right > 0)
        cameraPos +=
            glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
      if (left_right < 0)
        cameraPos -=
            glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
      view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    // Расчитываем перемещения всех объектов с учётом введённых в предыдущем
    // блоке интеракций.
    physics.Step();

    // pass projection matrix to shader (note that in this case it could change
    // every frame)
    glm::mat4 projection =
        glm::perspective(glm::radians(100.0f),
                         static_cast<float>(constants::screen_width) /
                             static_cast<float>(constants::screen_height),
                         0.1f, 100.0f);
    // make sure to initialize matrix to identity matrix first
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    float angle = 0.0f;
    model =
        glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.1f, 0.0f));

    renderer.UpdateCommon(&projection[0][0], &view[0][0], &model[0][0]);
    // for (const auto &cfg : cfgs) {
    //   const int key = cfg.first;
    //   renderer.UpdateDynamic(key, physics.CoordsSize(key),
    //                          physics.CoordsOLD(key), physics.NumShapes(key),
    //                          physics.AnglesSize(key), physics.AnglesOLD(key),
    //                          physics.ParamsSize(key),
    //                          physics.ParamsOLD(key));
    // }
    renderer.RenderFrame();
  }

  SDL_Quit();

  return 0;
}
