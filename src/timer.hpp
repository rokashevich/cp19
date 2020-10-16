#pragma once

#include <chrono>

using timestamp = std::chrono::time_point<std::chrono::steady_clock,
                                          std::chrono::milliseconds>;
class Timer
{
 public:
  // Зафиксировать отметку о прохождении круга (в цикле рендера, например).
  static void Step(int target_ticks);

 private:
  // Вспомогательная переменная для хранения отметки времени с предыдущего шага.
  static timestamp previous_timestamp_;

  static long accumulated_ticks_;
  static int accumulated_count_;
};
