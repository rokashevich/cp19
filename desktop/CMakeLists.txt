cmake_minimum_required(VERSION 3.10)
project(cp19)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
set (CMAKE_CXX_STANDARD 17)
set(SRC ${PROJECT_SOURCE_DIR}/../src)
set(TOOLS ${PROJECT_SOURCE_DIR}/../tools)
set(GENERATED ${PROJECT_SOURCE_DIR}/../generated)
file(MAKE_DIRECTORY ${GENERATED})

include(ExternalProject)
include(External_SDL2)

include_directories(${SDL2_DIR}/include/SDL2 ../glm ${GENERATED})
link_directories(${SDL2_DIR}/lib)
link_libraries(SDL2)
link_libraries(GL)
link_libraries(dl)

set(Shaders
  ${SRC}/pixel_missile.glsl
  ${SRC}/pixel_player.glsl
  ${SRC}/pixel_wall.glsl
  ${SRC}/vertex_missile.glsl
  ${SRC}/vertex_player.glsl
  ${SRC}/vertex_wall.glsl)

add_executable(cp19
  ${SRC}/main.cpp
  ${SRC}/world.cpp
  ${SRC}/object.cpp
  ${SRC}/object_missile.cpp
  ${SRC}/object_player.cpp
  ${SRC}/object_wall.cpp
  ${SRC}/point.hpp
  ${SRC}/vec.hpp
  ${SRC}/shader.hpp
  ${SRC}/physics.cpp
  ${SRC}/timer.cpp
  ${SRC}/generator_shape.cpp
  ${SRC}/generator_maze2d.cpp
  ${SRC}/shape.cpp
  ${SRC}/renderer_sdl.cpp
  ${SRC}/constants.hpp
  ${Shaders})

add_custom_target(vertex_wall ALL COMMAND
  python3 ${TOOLS}/embedfile.py
    vertex_wall
    ${SRC}/vertex_wall.glsl
    ${GENERATED}/vertex_wall.hpp)

add_custom_target(pixel_wall ALL COMMAND
  python3 ${TOOLS}/embedfile.py
    pixel_wall
    ${SRC}/pixel_wall.glsl
    ${GENERATED}/pixel_wall.hpp)

add_custom_target(vertex_missile ALL COMMAND
  python3 ${TOOLS}/embedfile.py
    vertex_missile
    ${SRC}/vertex_missile.glsl
    ${GENERATED}/vertex_missile.hpp)

add_custom_target(pixel_missile ALL COMMAND
  python3 ${TOOLS}/embedfile.py
    pixel_missile
    ${SRC}/pixel_missile.glsl
    ${GENERATED}/pixel_missile.hpp)

add_custom_target(vertex_player ALL COMMAND
  python3 ${TOOLS}/embedfile.py
    vertex_player
    ${SRC}/vertex_player.glsl
    ${GENERATED}/vertex_player.hpp)

add_custom_target(pixel_player ALL COMMAND
  python3 ${TOOLS}/embedfile.py
    pixel_player
    ${SRC}/pixel_player.glsl
    ${GENERATED}/pixel_player.hpp)

add_dependencies(cp19
  pixel_missile
  pixel_player
  pixel_wall
  vertex_missile
  vertex_player
  vertex_wall)
