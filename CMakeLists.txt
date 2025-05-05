cmake_minimum_required(VERSION 3.16)
project(PC-Club-Manager)

set(CMAKE_CXX_STANDARD 20)

add_executable(PC-Club-Manager src/main.cpp src/manager.cpp)
target_include_directories(PC-Club-Manager PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(extern/googletest)
add_executable(tests tests/tests.cpp)
target_link_libraries(tests gtest gtest_main)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
