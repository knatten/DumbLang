#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
source $SCRIPT_DIR/common.sh

if [ "$1" == "--fix" ]; then
  FLAGS="-i"
elif [ "$1" == "--check" ]; then
  FLAGS="--dry-run"
else
  echo "USAGE: $0 [--fix|--check]"
  exit 1
fi

find $SRC_DIR -name "*.h" -o -name "*.cpp" | xargs $CLANG_FORMAT $FLAGS --Werror
