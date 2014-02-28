export NACL_ROOT=/Users/hobu/Downloads/nacl_sdk/pepper_33
./cleanup.sh
cmake . -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-nacl.cmake -DBUILD_STATIC=ON -DCMAKE_BUILD_TYPE=Release
make VERBOSE=1
cp bin/pnacl_test.pexe pnacl/example
cp bin/pnacl_test.pexe.nmf pnacl/example/pnacl_test.nmf
