cmake_minimum_required(VERSION 3.20)
project(DumbLang)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -Wall")

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_BINARY_DIR})
find_package(argparse 2.9 REQUIRED CONFIG)
find_package(Catch2 2.13.9 REQUIRED CONFIG)
find_package(fmt 9.1.0 REQUIRED CONFIG)


enable_testing()

add_subdirectory(libdumblang)
add_subdirectory(dumblang)
