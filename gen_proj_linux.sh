#!/bin/bash

pushd "$(dirname "$0")" > /dev/null
cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Debug \
    -DBAREGL_GENERATE_EXAMPLES=ON
popd > /dev/null
