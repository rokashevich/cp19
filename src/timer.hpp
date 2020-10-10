#pragma once

#include <chrono>

using timestamp = std::chrono::time_point<std::chrono::steady_clock,
                                          std::chrono::microseconds>;
class Timer
{
 public:
  // Зафиксировать отметку о прохождении круга (в цикле рендера, например).
  static void Step();

  // Возвращает время последнего зафиксированного круга.
  static int Delta();

 private:
  // Кол-во наносекунд, прошедших с предыдущего шага (рендеринга кадра),
  // int должно хватить т.к. на андроиде это 2^31 = 2147483648 или 2 сек.
  static int delta_;

  // Вспомогательная переменная для хранения отметки времени с предыдущего шага.
  static timestamp previous_timestamp_;
};
