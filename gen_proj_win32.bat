@echo off

pushd "%~dp0"
cmake -S . -B build^
    -DBAREGL_GENERATE_EXAMPLES=ON
popd
