cmake_minimum_required(VERSION 3.20)
project(LibDumbLangTests)

add_executable(test_lib test_lexer.cpp test_parser.cpp test_main.cpp test_ast.cpp)
target_link_libraries(test_lib CONAN_PKG::catch2 dumblang)
add_test(NAME test_lib COMMAND test_lib)