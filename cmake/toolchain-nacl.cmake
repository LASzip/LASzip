#
# Google Native Client toolchain file for CMake
#
# (c) Copyrights 2009-2011 Hartmut Seichter
# 
# Note: only works on OSX
#

set(NACL_PATH $ENV{NACL_ROOT} CACHE STRING "Native Client SDK Root Path")


set(NACL_TAG pnacl CACHE STRING "")

set(NACL_HOST mac_x86_glibc CACHE STRING "")

if (APPLE)
	set(NACL_HOST mac_pnacl)
endif()


set( CMAKE_SYSTEM_NAME          "Generic" CACHE STRING "Target system." )
set( PLATFORM_EXE_SUFFIX        ".pexe" )
set( CMAKE_SYSTEM_PROCESSOR     "LLVM-IR" CACHE STRING "Target processor." )

# SET(CMAKE_SYSTEM_INCLUDE_PATH /include "${CMAKE_INSTALL_PREFIX}/include")
# SET(CMAKE_SYSTEM_LIBRARY_PATH /lib "${CMAKE_INSTALL_PREFIX}/lib")
# SET(CMAKE_SYSTEM_PROGRAM_PATH /bin "${CMAKE_INSTALL_PREFIX}/bin")
set( PLATFORM_EMBEDDED          ON )
#set(CMAKE_SYSTEM_PROCESSOR arm-eabi)

set(CMAKE_FIND_ROOT_PATH
    ${NACL_PATH}/toolchain/${NACL_HOST}
    )

set(CMAKE_ASM_COMPILER "${NACL_PATH}/toolchain/${NACL_HOST}/bin/${NACL_TAG}-as" CACHE STRING "")
set(CMAKE_C_COMPILER   "${NACL_PATH}/toolchain/${NACL_HOST}/bin/${NACL_TAG}-clang" CACHE STRING "")
set(CMAKE_CXX_COMPILER "${NACL_PATH}/toolchain/${NACL_HOST}/bin/${NACL_TAG}-clang++" CACHE STRING "")
set(CMAKE_AR "${NACL_PATH}/toolchain/${NACL_HOST}/bin/${NACL_TAG}-ar" CACHE STRING "")
set(CMAKE_C_LINK_EXECUTABLE "${NACL_PATH}/toolchain/${NACL_HOST}/bin/${NACL_TAG}-clang++ <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES> " CACHE STRING "")
#set(CMAKE_CXX_LINK_EXECUTABLE "${NACL_PATH}/toolchain/${NACL_HOST}/bin/${NACL_TAG}-clang++ <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES> " CACHE STRING "")
set(CMAKE_RANLIB "${NACL_PATH}/toolchain/${NACL_HOST}/bin/${NACL_TAG}-ranlib" CACHE STRING "")

INCLUDE (CMakeForceCompiler)
cmake_force_c_compiler(         ${CMAKE_C_COMPILER} Clang )
cmake_force_cxx_compiler(       ${CMAKE_CXX_COMPILER} Clang )



# set( CMAKE_C_FLAGS "-U__STRICT_ANSI__" CACHE STRING "" )
# set( CMAKE_CXX_FLAGS "-U__STRICT_ANSI__" CACHE STRING "" )
# set( CMAKE_C_FLAGS_RELEASE "-O4 -ffast-math" CACHE STRING "" )
# set( CMAKE_CXX_FLAGS_RELEASE "-O4 -ffast-math" CACHE STRING "" )
# 

message(STATUS "Getting LASzip build type - ${CMAKE_BUILD_TYPE}")
include_directories( SYSTEM $ENV{NACL_ROOT}/include )
link_directories( $ENV{NACL_ROOT}/lib/pnacl/Debug )
link_directories( $ENV{NACL_ROOT}/toolchain/mac_pnacl/usr/lib )
link_directories( $ENV{NACL_ROOT}/toolchain/mac_pnacl/sdk/lib )

macro( pnacl_compile _target files)
    add_custom_command( TARGET ${_target}
    PRE_BUILD COMMAND "${NACL_PATH}/toolchain/${NACL_HOST}/bin/${NACL_TAG}-clang++"
                       "$<TARGET_FILE:${_target}>" )
    # add_custom_command( TARGET ${_target}
    #  POST_BUILD COMMAND "python ${NACL_PATH}/tools/create_nmf.py"
    #                     "$<TARGET_FILE:${_target}>" )                       

endmacro()

macro( pnacl_finalise _target )
    add_custom_command( TARGET ${_target}
    POST_BUILD COMMAND "${NACL_PATH}/toolchain/${NACL_HOST}/bin/${NACL_TAG}-finalize"
                       "$<TARGET_FILE:${_target}>" )
                    

endmacro()


macro( pnacl_nmf _target )
    add_custom_command( TARGET ${_target}
     POST_BUILD COMMAND "python" "${NACL_PATH}/tools/create_nmf.py"
                        $<TARGET_FILE:${_target}> > $<TARGET_FILE:${_target}>.nmf VERBATIM)   

endmacro()
                    
set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )
set( CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY )
