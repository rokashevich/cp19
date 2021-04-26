cmake_minimum_required(VERSION 3.5)

project(net LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
# В Windows библиотека boost ставится вручную.
# Для Visual Studio 2015 качаем с
# https://sourceforge.net/projects/boost/files/boost-binaries/1.75.0/
# файл boost_1_75_0-msvc-14.0-64.exe. Обязательно 14.0!
# По сути это просто самораспаковывающийся zip архив. Запускаем его
# и устанавливаем в ту же директорию, в которую выкачан проект cp19.
# По умолчанию он предложит назвать директорию распаковки boost_1_75_0
# - переименовываем её просто в boost.
# Наконец, выставляем пути относительно проекта net до каталогов boost.
include_directories(${CMAKE_SOURCE_DIR}/../../boost)
link_directories(${CMAKE_SOURCE_DIR}/../../boost/lib64-msvc-14.0)
endif()

add_executable(client client.cpp interplay.hpp)
add_executable(server server.cpp interplay.hpp)

target_link_libraries(client -lboost_thread -lpthread)
target_link_libraries(server -lboost_thread -lpthread)
