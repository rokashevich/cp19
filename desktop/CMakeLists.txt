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
add_custom_command(
	OUTPUT  ${SRC}/vertex_shader.h
	COMMAND embedfile vertex_shader ${SRC}/shader.vert ${SRC}/vertex_shader.h
)
add_custom_command(
	OUTPUT  ${SRC}/fragment_shader.h
	COMMAND embedfile fragment_shader ${SRC}/shader.frag ${SRC}/fragment_shader.h
)
add_custom_target(shader_headers ALL DEPENDS ${SRC}/vertex_shader.h ${SRC}/fragment_shader.h)


add_executable(native-sdl-demo ../src/main.cpp)


