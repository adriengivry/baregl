@echo off

pushd "%~dp0"
cmake -G "Ninja Multi-Config" -B build^
    -DBAREGL_GENERATE_EXAMPLES=ON^
    -DBAREGL_GENERATE_TESTS=ON
popd
