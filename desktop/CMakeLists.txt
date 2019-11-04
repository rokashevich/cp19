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

add_custom_target(vertex_shader ALL COMMAND embedfile vertex_shader ${SRC}/shader.vert ${SRC}/vertex_shader.h)
add_custom_target(fragment_shader ALL COMMAND embedfile fragment_shader ${SRC}/shader.frag ${SRC}/fragment_shader.h)

add_executable(native-sdl-demo
	${SRC}/main.cpp
	${SRC}/shader.h
	${SRC}/shader.vert
	${SRC}/shader.frag)
