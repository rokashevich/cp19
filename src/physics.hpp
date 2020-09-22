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

  static void Step();
  static int Delta();

 private:
  static int step_;

  static std::list<Object*> is_;
  static std::list<Object*> os_;
  static std::list<Object*> ns_;

  static std::vector<float> render_parameters_i;
  static std::vector<float> render_parameters_o;
  static std::vector<float> render_parameters_n;

  static Physics::RenderParameters render_parameters_i_;
  static Physics::RenderParameters render_parameters_o_;
  static Physics::RenderParameters render_parameters_n_;
};