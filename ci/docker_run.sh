#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
source $SCRIPT_DIR/common.sh

REALPATH=realpath
if [ "$OS" == "mac" ]; then
  REALPATH=grealpath
fi

HOST_DIR=/host
WORKING_DIR="$HOST_DIR/$($REALPATH --relative-to=$ROOT_DIR $PWD)"
DOCKER_TAG=dumbdocker
DOCKER_ARGS=$@

echo "Building Docker tag '$DOCKER_TAG', running '$DOCKER_ARGS' in '$WORKING_DIR'"

cd $SCRIPT_DIR || exit $?
docker build . -t $DOCKER_TAG || exit $?

if [ $# -gt 0 ]; then
  docker run -v $SCRIPT_DIR/..:$HOST_DIR -w $WORKING_DIR $DOCKER_TAG bash -c "$DOCKER_ARGS" || exit $?
else
  docker run -it -v $SCRIPT_DIR/..:$HOST_DIR -w $WORKING_DIR $DOCKER_TAG || exit $?
fi
