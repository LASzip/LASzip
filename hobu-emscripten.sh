export EMSCRIPTEN=/Users/hobu/dev/git/emscripten
./cleanup.sh
cmake . -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-emscripten.cmake -DBUILD_STATIC=ON -DCMAKE_BUILD_TYPE=Debug
$EMSCRIPTEN/emmake make VERBOSE=1
