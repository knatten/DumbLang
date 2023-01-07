#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ROOT_DIR=$SCRIPT_DIR/..
BUILD_DIR=$ROOT_DIR/build/ci
SRC_DIR=$ROOT_DIR/src
CI_DIR=$ROOT_DIR/ci
C_COMPILER=clang-15
CXX_COMPILER=clang++-15
mkdir -p $BUILD_DIR || $?
cd $BUILD_DIR || $?
conan install $SRC_DIR --profile=$CI_DIR/conan-profiles/release  --build=missing || $?
cmake -S $SRC_DIR -B $BUILD_DIR -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER || $?
cmake --build $BUILD_DIR || $?
ctest || $?
