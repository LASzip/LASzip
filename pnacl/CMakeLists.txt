
set(PNACL_PROGRAM pnacl_test.pexe)

include_directories(
    .
    ../include
    $ENV{NACL_ROOT}/include
$ENV{NACL_ROOT}/include/pnacl)


add_executable(${PNACL_PROGRAM}  handlers.c queue.c hello.c)
target_link_libraries(${PNACL_PROGRAM} laszip )

link_directories( $ENV{NACL_ROOT}/toolchain/mac_pnacl/usr/lib )
# target_link_libraries(  ${PNACL_PROGRAM} 
#                          stdc++ c m g c++ nacl nacl_exception)

link_directories( $ENV{NACL_ROOT}/lib/pnacl/Debug )
target_link_libraries(  ${PNACL_PROGRAM} 
                         ppapi_cpp ppapi_simple ppapi_stub pthread nacl_io )



pnacl_finalise(${PNACL_PROGRAM})
pnacl_nmf(${PNACL_PROGRAM})
