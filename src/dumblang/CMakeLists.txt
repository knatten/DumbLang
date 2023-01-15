cmake_minimum_required(VERSION 3.20)
project(LibDumbLang)

add_executable(main main.cpp)
target_link_libraries(main dumblang CONAN_PKG::argparse)
set_target_properties(main PROPERTIES OUTPUT_NAME dumblang)