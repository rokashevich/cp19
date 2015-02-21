cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)
project(native-sdl-demo)


# workaround for out of tree build
set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/.)
add_subdirectory(../SDL ${CMAKE_BINARY_DIR}/build/sdl)

include_directories(../SDL/include)
link_directories(build/sdl)
link_libraries(SDL2)

add_executable(native-sdl-demo ../main.c)
