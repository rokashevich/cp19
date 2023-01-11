#include <EGL/egl.h>
#include <GLES3/gl32.h>
//#include <bits/stdint-intn.h>
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
//#include "../net/interplay.hpp"
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

int main(int, char **) {
  // Possible game bricks:
  //
  // Bricks of the same type have identical shape, but may differ in
  // sizes/colors. Rendering similar shapes is fast.
  //
  //   wall: parallelepipeds used to construct walls and floors
  //   player: parallelepipeds used to construct game characters
  //   weapon: cilinders used to construct guns/chainguns/pistols
  //   missile: spheres used to construct flying bullets/rockets/etc
  //   collectable: pyramids used to represend items across the level
  //   graffity: panels of different color you can "paint" on other surfaces
  enum { wall, player, weapon, missile, collectable, graffity };

  // Brick consists of:
  struct Brick {
    ShapeInfo static_info;  // geometry (a collection of dot coordinates),
    const bool is_dynamic;  // sign whether it moves (player, weapon,
                            // missile, graffity) or fixed (wall, collectable),
    const char *vertex_shader;  // shared shaders for batched rendering.
    const char *pixel_shader;
  };

  // Keep all bricks in one place - allocate storage for this here:
  std::unordered_map<int, Brick> bricks{
      {wall, {Shape<Wall>::StaticInfo(), false, vertex_wall, pixel_wall}},
      {player,
       {Shape<Player>::StaticInfo(), true, vertex_player, pixel_player}}};

  // Init/generate the maze.
  World game_world = World(constants::maze_dimension);

  // Init renderer.
  RendererSdl renderer;

  // Init physics engine.
  // Physical simulation engine is recalculating coordinates of all objects: how
  // much a bullet has moved, collision detection with player, etc.
  // Also, we pass in the render function (callback) - when coordinates
  // recalculation is done, the physics engine will ask the renderer to redraw.
  Physics physics([&](int key, int num_instances, const float *coords_data,
                      int coords_size, const float *angles_data,
                      int angles_size, const float *params_data,
                      int params_size) {
    renderer.OnNumInstancesChanged(key, num_instances, coords_data, coords_size,
                                   angles_data, angles_size, params_data,
                                   params_size);
  });

  // Iterate over all objects types (bricks types),
  // key is some of wall,player,weapon,missile,collectable,graffity
  // and brick is a brick type:
  for (const auto &[key, brick] : bricks) {
    physics.RegisterObject(key);
    renderer.RegisterObject(key, &brick.static_info.vertices_buffer,
                            brick.vertex_shader, brick.pixel_shader);
  }

  // Put walls coordiantes in physics engine:
  for (unsigned long i = 0; i < game_world.panels_data_array().size();) {
    float x = game_world.panels_data_array().at(i++);
    float y = game_world.panels_data_array().at(i++);
    float z = game_world.panels_data_array().at(i++);
    float a = game_world.panels_data_array().at(i++);
    float b = game_world.panels_data_array().at(i++);
    float c = game_world.panels_data_array().at(i++);
    float width = 0;   // game_world.panels_data_array().at(i++);
    float height = 0;  // game_world.panels_data_array().at(i++);
    float health = 0;  // game_world.panels_data_array().at(i++);
    i += 3;
    glm::vec3 angles = {a, b, c};
    glm::vec4 params = {width, height, health, 0.0f};
    Object *o = new Wall(glm::vec3{x, y, z}, angles);
    physics.AddObject(wall, o);
  }

  // Generate a player in the position 0,0,0 and put him into physics engine:
  auto const coords = glm::vec3{-3, -3, 5};
  auto const angles = glm::vec3{0, 0, 0};
  auto const head{1};
  auto const body{1};
  auto const arms{1};
  auto const legs{1};
  Object *player1 = new Player(coords, angles, head, body, arms, legs);
  physics.AddObject(player, player1);

  // Main game cycle:
  //
  // Simplified it is:
  //
  // while(1) {
  //  [1] get_inputs (move, turn, shoot)
  //  [2] physics_calculate_new_coordinats_for_everything
  //  [3] renderer_draws_everything
  // }
  //

  // Camera initial setup.
  static glm::vec3 cameraPos =
      glm::vec3(0.0f, 0.0f, .0f);  // отладка: позиция примерно сверху лабиринта
  static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  const float cameraSpeed = 0.2f;
  static float yaw = 0.0f;
  static float pitch = 0.0f;

  Object *controlled_object = player1;  // or nullptr for none
  Object *viewed_object = nullptr;      // or nullptr for freeflight
  bool quit = false;  // application will exit when flag set true
  while (!quit) {
    // [1]
    // Control variables filled in from user (or AI!) inputs:
    tribool backward_forward;  // walk forward, stay or backward
    tribool left_right;        // tend left or right, or stay straight
    tribool up_down;           // jump
    // TODO shoot
    // TODO plant bomb/explode

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&renderer.event)) {
      if (renderer.event.type == SDL_QUIT) {
        quit = true;
      }
#if __ANDROID__
// Here will go Android specific code.
#else  // PC code
      else if (renderer.event.type == SDL_KEYDOWN ||
               renderer.event.type == SDL_KEYUP) {

        if (state[SDL_SCANCODE_W]) ++backward_forward;
        if (state[SDL_SCANCODE_S]) --backward_forward;
        if (state[SDL_SCANCODE_A]) --left_right;
        if (state[SDL_SCANCODE_D]) ++left_right;
        if (state[SDL_SCANCODE_SPACE]) ++controlled_object->up_down;

        if (state[SDL_SCANCODE_T])
          viewed_object = controlled_object = viewed_object ? nullptr : player1;
      } else if (renderer.event.type == SDL_MOUSEMOTION && (!viewed_object)) {
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
#endif
    }

    yaw += 1.0f;
    pitch += 1.0f;
    if (yaw > 180.0f) yaw = 0.0f;
    if (pitch > 180.0f) pitch = 0.0f;
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

    if (controlled_object) {
      controlled_object->backward_forward = backward_forward;
      controlled_object->left_right = left_right;
      controlled_object->up_down = up_down;
    }

    // [2]
    physics.Step();

    // [3]
    // Преобразуем запрошенное управление из формата кнопок
    // вперёд-назад/влево-вправо/вверх в вектор в координатах игрового мира
    glm::mat4 view;
    // glm::mat4 trans = glm::mat4(1.0f);
    if (viewed_object) {  // Управление объектом.

      glm::vec3 followed{player1->coords.at(0), player1->coords.at(1),
                         player1->coords.at(2)};
      view = glm::lookAt(followed + glm::vec3{6, 6, 0}, followed, cameraUp);
    } else {  // Управление камерой.
      // backward_forward = -1;
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

    // for (const auto &cfg : bricks) {
    //   const int key = cfg.first;
    //   renderer.UpdateDynamic(key, physics.CoordsSize(key),
    //                          physics.CoordsOLD(key), physics.NumShapes(key),
    //                          physics.AnglesSize(key), physics.AnglesOLD(key),
    //                          physics.ParamsSize(key),
    //                          physics.ParamsOLD(key));
    // }
    renderer.RenderFrame(&projection[0][0], &view[0][0], &model[0][0]);
  }

  SDL_Quit();

  return 0;
}
