#pragma once

#include <list>

#include "object.hpp"
#include "timer.hpp"

class Object;

class Physics : protected Timer {
 public:
  struct RenderParameters {
    float* shader_data;
    int shader_data_size;
    int objects_count;
  };
  static void AddI(Object*);
  static void AddO(Object*);
  static void AddN(Object*);

  static const Physics::RenderParameters RenderParametersO();
  static const Physics::RenderParameters RenderParametersN();

  static void Step();

 private:
  static int target_fps_;
  static int step_ticks_;

  static std::list<Object*> is_;
  static std::list<Object*> os_;
  static std::list<Object*> ns_;

  static std::vector<float> render_parameters_i;
  static std::vector<float> render_parameters_o;
  static std::vector<float> render_parameters_n;
};
