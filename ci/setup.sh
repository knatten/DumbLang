#!/bin/bash

if [ $UID != 0 ]; then
  SUDO=sudo
else
  SUDO=""
fi

$SUDO apt-get update || exit $?
$SUDO apt-get install --assume-yes lsb-release software-properties-common wget pip cmake|| exit $?
TMP_DIR=$(mktemp -d)
pushd $TMP_DIR || exit $?
wget https://apt.llvm.org/llvm.sh || exit $?
chmod +x llvm.sh || exit $?
$SUDO ./llvm.sh 15|| exit $?
$SUDO apt install clang-format-15 || exit $?
pip install "conan==2.0.0b8" || exit $?