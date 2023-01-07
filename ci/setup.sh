#!/bin/bash

sudo apt-get install --assume-yes lsb-release software-properties-common || exit $?
TMP_DIR=$(mktemp -d)
pushd $TMP_DIR || exit $?
wget https://apt.llvm.org/llvm.sh || exit $?
chmod +x llvm.sh || exit $?
sudo ./llvm.sh 15|| exit $?
pip install conan || exit %?