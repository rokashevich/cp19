#include "timer.hpp"

#include <iostream>

int Timer::delta_{0};
timestamp Timer::previous_timestamp_ =
    std::chrono::time_point_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now());

void Timer::Step() {
  const auto current_timestamp =
      std::chrono::time_point_cast<std::chrono::microseconds>(
          std::chrono::steady_clock::now());
  Timer::delta_ = std::chrono::duration_cast<std::chrono::microseconds>(
                      current_timestamp - Timer::previous_timestamp_)
                      .count();
  Timer::previous_timestamp_ = current_timestamp;
}

int Timer::Delta() { return Timer::delta_; }
