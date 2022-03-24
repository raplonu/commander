#!/bin/bash
# Project build directory relative to current directory.
BUILD_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )/build"

# Exit on first error.
set -e

mkdir -p $BUILD_DIR
cd $BUILD_DIR

conan install .. --build=missing -pr default -pr:b=default $@
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_MODULE_PATH=.
