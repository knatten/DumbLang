#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
source $SCRIPT_DIR/common.sh

BUILD_DIR=$ROOT_DIR/build/ci/$OS


mkdir -p $BUILD_DIR || exit $?
cd $BUILD_DIR || exit $?
conan install $SRC_DIR -pr=$CI_DIR/conan-profiles/$OS/release --build=missing -of=. || exit $?
cmake -S $SRC_DIR -B $BUILD_DIR -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER -DCMAKE_BUILD_TYPE=Release || exit $?
cmake --build $BUILD_DIR || exit $?

ctest || exit $?
$BUILD_DIR/dumblang/dumblang --dump-ast ../../../examples/example.dumb || exit $? # Just check that it runs
