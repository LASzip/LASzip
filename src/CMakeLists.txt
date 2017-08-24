###############################################################################
#
# src/CMakeLists.txt controls building of laszip library
#
# Copyright (c) 2009 Mateusz Loskot <mateusz@loskot.net>
#
###############################################################################
set(LASZIP_SOURCES
    arithmeticdecoder.cpp
    arithmeticencoder.cpp
    arithmeticmodel.cpp
    integercompressor.cpp
    lasindex.cpp
    lasinterval.cpp
    lasquadtree.cpp
    lasreaditemcompressed_v1.cpp
    lasreaditemcompressed_v2.cpp
    lasreaditemcompressed_v3.cpp
    lasreadpoint.cpp
    #lasunzipper.cpp
    laswriteitemcompressed_v1.cpp
    laswriteitemcompressed_v2.cpp
    laswriteitemcompressed_v3.cpp
    laswritepoint.cpp
    laszip.cpp
    laszip_dll.cpp
)

add_definitions(-DUNORDERED)
add_definitions(-DLASZIPDLL_EXPORTS)

LASZIP_ADD_LIBRARY(${LASZIP_BASE_LIB_NAME} ${LASZIP_SOURCES})
