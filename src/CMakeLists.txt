cmake_minimum_required(VERSION 3.20)
project(DumbLang)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -Wall")

# Conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(dumblang lexer.cpp parser.cpp)
target_include_directories(dumblang PUBLIC include)

add_executable(main main.cpp)
target_link_libraries(main dumblang)

add_executable(test test_lexer.cpp test_parser.cpp)
target_link_libraries(test Catch2Main Catch2 dumblang)

