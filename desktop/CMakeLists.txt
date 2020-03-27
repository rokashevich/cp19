cmake_minimum_required(VERSION 2.8)
project(native-sdl-demo)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
set (CMAKE_CXX_STANDARD 11)
set(SRC ${PROJECT_SOURCE_DIR}/../src)

include(ExternalProject)
include(External_SDL2)

include_directories(${SDL2_DIR}/include/SDL2 ../glm)
link_directories(${SDL2_DIR}/lib)
link_libraries(SDL2)
link_libraries(GL)
link_libraries(dl)

add_executable(embedfile ../tools/embedfile.cpp)

add_custom_target(vertex_shader_panel ALL COMMAND embedfile vertex_shader
  ${SRC}/vertex_shader_panel.glsl ${SRC}/vertex_shader_panel.h)
add_custom_target(vertex_shader_rib ALL COMMAND embedfile vertex_shader
  ${SRC}/vertex_shader_rib.glsl ${SRC}/vertex_shader_rib.h)
add_custom_target(fragment_shader ALL COMMAND embedfile fragment_shader
  ${SRC}/frag_cube.glsl ${SRC}/fragment_shader.h)

add_executable(native-sdl-demo
  ${SRC}/main.cpp
  ${SRC}/gameworld.cpp
  ${SRC}/helpers/maze2d.cpp
  ${SRC}/shader.hpp
  ${SRC}/vertex_shader_panel.glsl
  ${SRC}/vertex_shader_rib.glsl
  ${SRC}/frag_cube.glsl)

add_dependencies(native-sdl-demo
  vertex_shader_panel
  vertex_shader_rib
  fragment_shader)
