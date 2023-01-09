#include "timer.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

timestamp Timer::previous_timestamp_ =
    std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now());

long Timer::accumulated_ticks_{0};
int Timer::accumulated_count_{0};

void Timer::SleepAlign(int target_ticks) {
  const auto delta_ticks =
      std::chrono::duration(
          std::chrono::time_point_cast<std::chrono::milliseconds>(
              std::chrono::steady_clock::now()) -
          previous_timestamp_)
          .count();

  const auto sleep_ticks = target_ticks - delta_ticks;
  if (sleep_ticks > 0)
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ticks));

  Timer::previous_timestamp_ =
      std::chrono::time_point_cast<std::chrono::milliseconds>(
          std::chrono::steady_clock::now());

  // FPS.
  if (accumulated_count_ > 100) {
    const long average_delta =
        Timer::accumulated_ticks_ / Timer::accumulated_count_;
    const int fps = 1000 / average_delta;
    std::cout << "current delta: " << delta_ticks
              << " average delta: " << average_delta << " fps: " << fps
              << std::endl;
    Timer::accumulated_ticks_ = Timer::accumulated_count_ = 0;
  } else {
    //    std::cout << "else" << std::endl;
    Timer::accumulated_ticks_ += delta_ticks;
    ++Timer::accumulated_count_;
  }
}
