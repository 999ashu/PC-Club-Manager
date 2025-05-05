cmake_minimum_required(VERSION 3.16)
project(Computer-Cafe-Manager)

set(CMAKE_CXX_STANDARD 20)

add_executable(Computer-Cafe-Manager src/main.cpp src/manager.cpp)
target_include_directories(Computer-Cafe-Manager PRIVATE ${CMAKE_SOURCE_DIR}/include)
