cmake_minimum_required(VERSION 2.8)
project(native-sdl-demo)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
set (CMAKE_CXX_STANDARD 11)

include(ExternalProject)
include(External_SDL2)

include_directories(${SDL2_DIR}/include/SDL2 ../glm)
link_directories(${SDL2_DIR}/lib)
link_libraries(SDL2)
link_libraries(GL)
link_libraries(dl)

add_executable(native-sdl-demo ../src/main.cpp)


