set(LASZIP_COMMON_CXX_FLAGS "-Wextra -Wall -Wno-unused-parameter -Wno-unused-variable -Wpointer-arith -Wcast-qual -Wredundant-decls -Wno-long-long -Wno-unknown-pragmas -isystem /usr/local/include"
)

if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER 4.6)
        set(LASZIP_NO_AS_NEEDED_START "-Wl,--no-as-needed")
        set(LASZIP_NO_AS_NEEDED_END "-Wl,--as-needed")
    endif()
    set(LASZIP_COMPILER_GCC 1)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(LASZIP_COMPILER_CLANG 1)
else()
    message(FATAL_ERROR "Unsupported C++ compiler")
endif()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LASZIP_COMMON_CXX_FLAGS}")
