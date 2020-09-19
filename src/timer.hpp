#pragma once

class Timer
{
 public:
  // Зафиксировать отметку о прохождении круга (в цикле рендера, например).
  static void Step();

  // Возвращает время последнего зафиксированного круга.
  static int Delta();

 private:
  static unsigned int last_ticks_;
  static int delta_;
};
