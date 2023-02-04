#!/bin/bash

if [ $UID != 0 ]; then
  SUDO=sudo
else
  SUDO=""
fi

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )


$SUDO apt-get update || exit $?
$SUDO apt-get install --assume-yes lsb-release software-properties-common wget pip cmake|| exit $?
pip install -r $SCRIPT_DIR/requirements.txt || exit $?
TMP_DIR=$(mktemp -d)
pushd $TMP_DIR || exit $?
wget https://apt.llvm.org/llvm.sh || exit $?
chmod +x llvm.sh || exit $?
$SUDO ./llvm.sh 15|| exit $?
$SUDO apt install clang-format-15 || exit $?