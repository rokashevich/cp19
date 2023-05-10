// x +влево -вправо
// y +вверх -вниз
// z +назад -вперёд

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

// Заголовочные файлы проекта
//#include "../net/interplay.hpp"
#include "constants.hpp"
#include "generator_shape.hpp"
#include "group.hpp"
#include "object.hpp"
#include "physics.hpp"
#include "player.hpp"
#include "renderer_sdl.hpp"
#include "shader.hpp"
#include "t.hpp"
#include "wall.hpp"
#include "world.hpp"
// Шейдеры
#include "pixel_player.hpp"
#include "pixel_wall.hpp"
#include "vertex_player.hpp"
#include "vertex_wall.hpp"
int main(int, char **) {
  // Стены, платформы, игроки, оружие, снаряды, артифакты, граффити
  // - все объекты игрового мира в одном месте.
  std::vector<Group> groups;

  // Сгенерируем лабиринт и закинем стены в пока пустую структуру.
  float panel_size = 2.9;
  float panel_thikness = 0.05;
  float world_panel_size = panel_size + 2 * panel_thikness;
  World game_world = World(constants::maze_dimension, world_panel_size);
  Group walls{true, vertex_wall, pixel_wall,
              ShapeGenerator::Cuboid(panel_size, panel_size, panel_thikness)};
  for (unsigned long i = 0; i < game_world.panels_data_array().size();) {
    float x = game_world.panels_data_array().at(i++);
    float y = game_world.panels_data_array().at(i++);
    float z = game_world.panels_data_array().at(i++);
    float a = game_world.panels_data_array().at(i++);
    float b = game_world.panels_data_array().at(i++);
    float c = game_world.panels_data_array().at(i++);
    i += 3;
    glm::vec3 angles = {a, b, c};
    Object *o = new Wall(glm::vec3{x, y, z}, angles);
    walls.objects.push_back(o);
  }
  groups.push_back(walls);

  // Создаём игрока в произвольном месте и аналогично закидываем в структуру.
  Group players{false, vertex_player, pixel_player,
                ShapeGenerator::Cuboid(3, 3, 3)};
  auto const coords = glm::vec3{0, 16, 0};
  auto const angles = glm::vec3{0, 0, 0};
  auto const head{1};
  auto const body{1};
  auto const arms{1};
  auto const legs{1};
  Object *player1 = new Player(coords, angles, head, body, arms, legs);
  players.objects.push_back(player1);
  groups.push_back(players);

  // Настройки камеры для полётов по уровню без управления игроком.
  static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
  static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  const float cameraSpeed = 0.2f;
  static float yaw = 0.0f;
  static float pitch = 0.0f;

  Object *controlled_object = player1;  //кем будем управлять
  Object *viewed_object = player1;  //за кем будет следить камера
  // Какие это даёт комбинации:
  // 1) Обе переменные указывают на игрока - обычный режим игры;
  // 2) Обе переменные nullptr - режим свободного полёта/просмотра карты;
  // 3) Контроллируем nullptr, следим за игроком - режим просмотра чей-то игры;
  // Остальные комбинации бессмысленны :)

  // В самом конце инициаллизируем физический движок и рендерер, передавая им
  // структуру объектов мира - т.к. они должны о ней знать.
  Physics physics(&groups);
  RendererSdl renderer(&groups);

  // Далее начинается классический бесконечный игровой цикл,
  // по сути while(1){считаем/рисуемкадр}.

  bool quit = false;  //для выхода из вложенных while из игрового цикла
  while (!quit) {
    // В начале каждого кадра подготавливаем/обнуляем переменные для управления
    // игроком. По типу это т.н. трёхсостоянный бул: 1,0,-1.
    tribool forth_back;  //вперёд,стоять,назад
    tribool left_right;  //влево,стоять,вправо
    tribool up_down;     //вверх,стоять,вниз
    // TODO Добавить другие переменные для стрельбы например.

    // Далее стандартный цикл библиотеки SDL по произошедшим событиям.
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&renderer.event)) {
      if (renderer.event.type == SDL_QUIT) {
        quit = true;
      }
#if __ANDROID__
      // Датчик пространственной ориентации телефона.
      // Проверял на:
      //   realme c30, xiaomi a1
      // Значения "некрсивые" типа 0.167500 -0.110000 9.809999.
      // Порядок значений:       x   y  z
      //          Лежит ровно:   0   0 10
      //        Наклон вперёд: -10   0  0
      //         Наклон назад:  10   0  0
      //         Наклон влево:   0 -10  0
      //        Наклон вправо:   0  10  0
      // Иногда приходит какой-то шум, например, для ровнолежачего:
      //   69.375000 0.000000 0.000000
      //    1.000000 0.000000 0.000000
      //    5.000000 0.000000 0.000000
      //    0.000000 0.000000 0.000000
      //   70.000000 0.000000 0.000000
      // Пока не знаю что это, но в шумных занчениях обязательно присутствуют
      // асболютные нули - попробую их игнорировать просто.
      else if (renderer.event.type == SDL_SENSORUPDATE) {
        const auto x = renderer.event.sensor.data[0];
        const auto y = renderer.event.sensor.data[1];
        const auto z = renderer.event.sensor.data[2];
        const auto treshold = 2;  //если меньше считаем телефон в горизонте
        const auto max = 10;  //если больше - шум с датчика, игнорим такое
        if (x != 0.0f && y != 0.0f && z != 0.0f) {  // асболютные нули
          if (x < -treshold) ++forth_back;  // вперёд
          if (x > treshold) --forth_back;   // назад
          if (y < -treshold) --left_right;        // влево
          if (y > treshold) ++left_right;         // вправо
        }
      }
#else  // PC code
      else if (renderer.event.type == SDL_KEYDOWN ||
               renderer.event.type == SDL_KEYUP) {

        // На PC управляемся классическим WSAD, еще RF - вверх,вниз.
        // За нулевое состояние отвечает - ненажатие на клавиши.
        if (state[SDL_SCANCODE_W]) ++forth_back;
        if (state[SDL_SCANCODE_S]) --forth_back;
        if (state[SDL_SCANCODE_A]) --left_right;
        if (state[SDL_SCANCODE_D]) ++left_right;
        if (state[SDL_SCANCODE_R]) ++up_down;
        if (state[SDL_SCANCODE_F]) --up_down;

        // T - переключает между контролем/просмотром игрока и свободным
        // просмотром карты.
        if (state[SDL_SCANCODE_T])
          viewed_object = controlled_object = viewed_object ? nullptr : player1;
      } else if (renderer.event.type == SDL_MOUSEMOTION && (!viewed_object)) {
        // Какой-то код для возможности крутить головой мышкой. Аналога на
        // телефоне нет, т.о. это больше для отладки и, наверное, можно это
        // будет вообще удалить.
        static float sensitivity = 0.1f;
        yaw += renderer.event.motion.xrel * sensitivity;
        pitch -= renderer.event.motion.yrel * sensitivity;
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

    // Не важно, как получено управление, с телефона или с клавиатуры -
    // выставляем управляемому объекту запрос на перемещение. Но не перемещаем!
    // Это будет делать физический движок!
    if (controlled_object) {
      controlled_object->backward_forward = forth_back;
      controlled_object->left_right = left_right;
      controlled_object->up_down = up_down;
    }

    // Шаг физического движка: посмотреть кто куда хочет переместиться,
    // рассчитать столкновения, кого-то переместить. Будет изменена структура
    // физического мира короче.
    physics.Step();

    // Структура физического мира изменена - время отрисовать новый кадр.

    // Сначала изменим положение камеры наблюдателя.
    glm::mat4 view;
    if (viewed_object) {  //есть привязанный объект
      // Классический режим игры от тртеьго лица TPS (Third Person Shooter) -
      // камера следует за игроком.
      glm::vec3 followed{player1->coords};
      view = glm::lookAt(followed + glm::vec3{0, 6, -6}, followed, cameraUp);
    } else {  //привязанного объекта нет - значит просто летаем по уровню
      if (forth_back > 0) cameraPos += cameraSpeed * cameraFront;
      if (forth_back < 0) cameraPos -= cameraSpeed * cameraFront;
      if (left_right > 0)
        cameraPos +=
            glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
      if (left_right < 0)
        cameraPos -=
            glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
      view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    // Наконец, рендерер, имея доступ к структре мировых объектов их рисует!
    renderer.RenderFrame(&view[0][0]);
  }

  SDL_Quit();

  return 0;
}
