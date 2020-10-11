#include "timer.hpp"

#include <chrono>
#include <iostream>
#include <thread>

timestamp Timer::previous_timestamp_ =
    std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now());

void Timer::Step(int target_ticks) {
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
}
