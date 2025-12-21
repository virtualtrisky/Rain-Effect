cmake_minimum_required(VERSION 3.31)
project(RainEffect)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(RainEffect include)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(RainEffect src/main.cpp)

target_link_libraries(RainEffect ${SDL2_LIBRARIES})
