cmake_minimum_required(VERSION 3.20)
project(LibDumbLang)

add_library(dumblang lexer.cpp parser.cpp ast_dumper.cpp)
target_include_directories(dumblang PUBLIC include)
target_link_libraries(dumblang fmt::fmt)

add_subdirectory(tests)