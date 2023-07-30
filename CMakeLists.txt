cmake_minimum_required(VERSION 3.8)

project(Game)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DUNICODE)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)