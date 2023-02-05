cmake_minimum_required(VERSION 3.20)
project(LibDumbLang)

add_library(dumblang lexer.cpp parser.cpp ast_dumper.cpp module_impl.cpp module.cpp)
target_include_directories(dumblang PUBLIC include)


if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(PACKAGE_PATH_LLVM /opt/homebrew/Cellar/llvm/15.0.7/lib/cmake/llvm/)
else()
    set(PACKAGE_PATH_LLVM /usr/lib/llvm-15/lib/cmake/llvm)
endif()
find_package(LLVM 15 REQUIRED CONFIG PATHS ${PACKAGE_PATH_LLVM} NO_DEFAULT_PATH)
separate_arguments(LLVM_DEFINITIONS)
add_library(LLVM_WRAPPER INTERFACE)
target_include_directories(LLVM_WRAPPER SYSTEM INTERFACE ${LLVM_INCLUDE_DIRS})
target_link_libraries(LLVM_WRAPPER INTERFACE LLVM${ARCHITECTURE}AsmParser LLVM${ARCHITECTURE}CodeGen )
target_compile_definitions(LLVM_WRAPPER INTERFACE ${LLVM_DEFINITIONS})

add_executable(test_compile compile.cpp)
target_link_libraries(test_compile PRIVATE LLVM_WRAPPER fmt::fmt dumblang)
target_link_libraries(dumblang PRIVATE LLVM_WRAPPER fmt::fmt)

add_subdirectory(tests)