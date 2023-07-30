cmake_minimum_required(VERSION 3.8)

project(Game)

set(CMAKE_CXX_STANDARD_REQUIRED 17)

add_definitions(-DUNICODE)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)