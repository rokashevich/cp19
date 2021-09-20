cmake_minimum_required(VERSION 3.5)

project(tests)

include_directories(
  googletest/googletest/include
  googletest/googletest
  googletest/googlemock/include
  googletest/googlemock
  ../src
)

file(GLOB TestsSources "*.cpp")

add_executable(tests
  googletest/googletest/src/gtest-all.cc
  googletest/googlemock/src/gmock-all.cc
  ${TestsSources}
)

target_link_libraries(tests -lpthread)
