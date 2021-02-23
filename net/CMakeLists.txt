cmake_minimum_required(VERSION 3.5)

project(net LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(client client.cpp interplay.hpp)
add_executable(server server.cpp interplay.hpp)

target_link_libraries(client -lboost_thread -lpthread)
target_link_libraries(server -lboost_thread -lpthread)
