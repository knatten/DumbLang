cmake_minimum_required(VERSION 3.20)
project(Kaleidoscope)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -Wall")

# Conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Executable
add_executable(main main.cpp lexer.cpp)

add_executable(test test_lexer.cpp lexer.cpp)
target_link_libraries(test Catch2Main Catch2)

