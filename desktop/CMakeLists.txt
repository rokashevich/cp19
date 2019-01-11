cmake_minimum_required(VERSION 2.8)
project(native-sdl-demo)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

include(ExternalProject)
include(External_SDL2)

include_directories(${SDL2_DIR}/include/SDL2)
link_directories(${SDL2_DIR}/lib)
link_libraries(SDL2)
link_libraries(GL)

add_executable(native-sdl-demo ../main.c)


