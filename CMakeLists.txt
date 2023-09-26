cmake_minimum_required(VERSION 3.8)

project(Game
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DUNICODE)

add_compile_options(/utf-8)

#include_directories(include)

add_subdirectory(external)

add_subdirectory(shader)

add_subdirectory(src)