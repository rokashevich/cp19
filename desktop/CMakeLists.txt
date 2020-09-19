cmake_minimum_required(VERSION 3.10)
project(native-sdl-demo)

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
  ${SRC}/shader_fragment_panel.glsl
  ${SRC}/shader_vertex_panel.glsl
  ${SRC}/shader_fragment_missile.glsl
  ${SRC}/shader_vertex_missile.glsl)

add_executable(native-sdl-demo
  ${SRC}/main.cpp
  ${SRC}/gameworld.cpp
  ${SRC}/helpers/maze2d.cpp
  ${SRC}/point.hpp
  ${SRC}/vec.hpp
  ${SRC}/shader.hpp
  ${SRC}/object.cpp
  ${SRC}/physics.cpp
  ${SRC}/timer.cpp
  ${Shaders})

add_custom_target(shader_vertex_panel ALL COMMAND
  python3 ${TOOLS}/embedfile.py
    shader_vertex_panel
    ${SRC}/shader_vertex_panel.glsl
    ${GENERATED}/shader_vertex_panel.hpp)

add_custom_target(shader_fragment_panel ALL COMMAND
  python3 ${TOOLS}/embedfile.py
    shader_fragment_panel
    ${SRC}/shader_fragment_panel.glsl
    ${GENERATED}/shader_fragment_panel.hpp)

add_custom_target(shader_vertex_missile ALL COMMAND
  python3 ${TOOLS}/embedfile.py
    shader_vertex_missile
    ${SRC}/shader_vertex_missile.glsl
    ${GENERATED}/shader_vertex_missile.hpp)

add_custom_target(shader_fragment_missile ALL COMMAND
  python3 ${TOOLS}/embedfile.py
    shader_fragment_missile
    ${SRC}/shader_fragment_missile.glsl
    ${GENERATED}/shader_fragment_missile.hpp)

add_dependencies(native-sdl-demo
  shader_vertex_panel
  shader_fragment_panel
  shader_vertex_missile
  shader_fragment_missile)

