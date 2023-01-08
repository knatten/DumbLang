#!/bin/bash

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        OS=linux
elif [[ "$OSTYPE" == "darwin"* ]]; then
        OS=mac
else
  echo "Unknown OS $OSTYPE"
  exit 1
fi

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ROOT_DIR=$SCRIPT_DIR/..
BUILD_DIR=$ROOT_DIR/build/ci/$OS
SRC_DIR=$ROOT_DIR/src
CI_DIR=$ROOT_DIR/ci
C_COMPILER=clang-15
CXX_COMPILER=clang++-15

mkdir -p $BUILD_DIR || exit $?
cd $BUILD_DIR || exit $?
conan install $SRC_DIR --profile=$CI_DIR/conan-profiles/$OS/release  --build=missing || exit $?
cmake -S $SRC_DIR -B $BUILD_DIR -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER || exit $?
cmake --build $BUILD_DIR || exit $?

ctest || exit $?
$BUILD_DIR/bin/dumblang ../../../examples/example.dumb || exit $? # Just check that it runs
