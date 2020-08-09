#ifndef OBJECT_H
#define OBJECT_H

// Базовый класс физического объекта игрового мира.
class Object
{
 public:
  Object(int weight, float x, float y, float z);
  virtual ~Object() {}

 protected:
  int weight_;
  float x_;
  float y_;
  float z_;
};

// Снаряд. Все снаряды одинаковые, отличаются размером, скоростью и,
// соответственно, убойной силой.
class Missile : public Object {
 public:
  Missile(int diameter, float x, float y, float z);
  ~Missile();
  inline constexpr static float vertices[] = {
      -0.05, -0.05, -0.05, -0.05, -0.05,
      0.05,  0.05,  -0.05, -0.05,  // Нижняя крышка.
      0.05,  -0.05, 0.05,  -0.05, -0.05,
      0.05,  0.05,  -0.05, -0.05,

      -0.05, 0.05,  -0.05, -0.05, 0.05,
      0.05,  0.05,  0.05,  -0.05,  // Верхняя крышка.
      0.05,  0.05,  0.05,  -0.05, 0.05,
      0.05,  0.05,  0.05,  -0.05,

      -0.05, -0.05, 0.05,  -0.05, 0.05,
      0.05,  0.05,  -0.05, 0.05,  // Стенка xy.
      0.05,  0.05,  0.05,  -0.05, 0.05,
      0.05,  0.05,  -0.05, 0.05,

      0.05,  -0.05, -0.05, 0.05,  0.05,
      -0.05, 0.05,  -0.05, 0.05,  // Стенка zy.
      0.05,  0.05,  0.05,  0.05,  0.05,
      -0.05, 0.05,  -0.05, 0.05,

      -0.05, -0.05, -0.05, -0.05, 0.05,
      -0.05, 0.05,  -0.05, -0.05,  // Стенка -xy.
      0.05,  0.05,  -0.05, -0.05, 0.05,
      -0.05, 0.05,  -0.05, -0.05,

      -0.05, -0.05, -0.05, -0.05, 0.05,
      -0.05, -0.05, -0.05, 0.05,  // Стенка -zy.
      -0.05, 0.05,  0.05,  -0.05, 0.05,
      -0.05, -0.05, -0.05, 0.05,
  };

 private:
  int diameter_;
};

class Player : public Object {
 public:
  Player(int head, int body, int hands, int legs, int guns, float x, float y,
         float z);
  ~Player();

 private:
  int head_;
  int body_;
  int hands_;
  int legs_;
  int guns_;
};

#endif  // OBJECT_H
