cmake_minimum_required(VERSION 3.20)
project(LibDumbLang)

add_library(dumblang lexer.cpp parser.cpp)
target_include_directories(dumblang PUBLIC include)

add_subdirectory(tests)