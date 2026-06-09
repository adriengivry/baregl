#!/bin/bash

pushd "$(dirname "$0")" > /dev/null
cmake -G "Ninja Multi-Config" -B build \
    -DBAREGL_GENERATE_EXAMPLES=ON \
    -DBAREGL_GENERATE_TESTS=ON
popd > /dev/null
