#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
source $SCRIPT_DIR/common.sh

if [ $# != 1 ]; then
  echo "USAGE: $0 <Debug|Release|All>"
  exit 1
fi

CONFIG=$1

case "$CONFIG" in
  Debug|Release) echo "yes";;
  All) CONFIG="Debug Release";;
  *) echo "Unknown config '$CONFIG'"; exit 1;;
esac

echo "Config is $CONFIG"

UNAME_ARCH=$(uname -m) || exit $?
if [ "$UNAME_ARCH" == "arm64" ] || [ "$UNAME_ARCH" == "aarch64" ]; then
    ARCHITECTURE=AArch64
elif [ "$UNAME_ARCH" == "x86_64" ]; then
    ARCHITECTURE=X86
else
    echo "Unsupported architecture ${UNAME_ARCH}"
    exit 1;
fi

for CFG in $CONFIG; do
  echo "Building in $CFG on $ARCHITECTURE"
  BUILD_DIR=$ROOT_DIR/build/ci/$OS/$CFG

  mkdir -p $BUILD_DIR || exit $?
  cd $BUILD_DIR || exit $?
  conan install $SRC_DIR -pr:b $CI_DIR/conan-profiles/$OS/$CFG -pr:h $CI_DIR/conan-profiles/$OS/$CFG --build missing -of . || exit $?
  cmake -S $SRC_DIR -B $BUILD_DIR -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER -DCMAKE_BUILD_TYPE=$CFG -DARCHITECTURE=$ARCHITECTURE || exit $?
  cmake --build $BUILD_DIR || exit $?

  ctest || exit $?
  $BUILD_DIR/dumblang/dumblang --dump-ast ../../../../examples/example.dumb || exit $? # Just check that it runs
  $BUILD_DIR/libdumblang/test_compile --dump-ast || exit $? # Just check that it runs
done
