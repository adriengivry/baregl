#!/bin/bash

pushd "$(dirname "$0")" > /dev/null
cmake -S . -B build \
    -DBAREGL_GENERATE_EXAMPLES=ON
popd > /dev/null
