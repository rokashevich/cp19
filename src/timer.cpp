#include "timer.hpp"

#include "SDL.h"

unsigned int Timer::last_ticks_ = 0;
int Timer::delta_ = 1;

void Timer::Step() {
  const unsigned int current_ticks = SDL_GetTicks();
  Timer::delta_ = current_ticks >= Timer::last_ticks_
                      ? current_ticks - Timer::last_ticks_
                      : 1;
  Timer::last_ticks_ = current_ticks;
}

int Timer::Delta() { return Timer::delta_; }
