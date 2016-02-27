###############################################################################
#
# src/CMakeLists.txt controls building of laszip library
#
# Copyright (c) 2009 Mateusz Loskot <mateusz@loskot.net>
#
###############################################################################
set(LASZIP_HEADERS_DIR ../include/laszip)

set(LASZIP_HPP
    arithmeticdecoder.hpp
    arithmeticencoder.hpp
    arithmeticmodel.hpp
    bytestreamin.hpp
    bytestreamin_array.hpp
    bytestreamin_file.hpp
    bytestreamin_istream.hpp
    bytestreaminout.hpp
    bytestreaminout_file.hpp
    bytestreamout.hpp
    bytestreamout_array.hpp
    bytestreamout_file.hpp
    bytestreamout_nil.hpp
    bytestreamout_ostream.hpp
    endian.hpp
    integercompressor.hpp
    lasattributer.hpp
    lasindex.hpp
    lasinterval.hpp
    laspoint.hpp
    lasquadtree.hpp
    lasquantizer.hpp
    lasreaditem.hpp
    lasreaditemcompressed_v1.hpp
    lasreaditemcompressed_v2.hpp
    lasreaditemraw.hpp
    lasreadpoint.hpp
    #lasunzipper.hpp
    laswriteitem.hpp
    laswriteitemcompressed_v1.hpp
    laswriteitemcompressed_v2.hpp
    laswriteitemraw.hpp
    laswritepoint.hpp
    laszip.hpp
    laszip_common_v1.hpp
    laszip_common_v2.hpp
    mydefs.hpp
 )

set(LASZIP_CPP
    arithmeticdecoder.cpp
    arithmeticencoder.cpp
    arithmeticmodel.cpp
    integercompressor.cpp
    lasindex.cpp
    lasinterval.cpp
    lasquadtree.cpp
    lasreaditemcompressed_v1.cpp
    lasreaditemcompressed_v2.cpp
    lasreadpoint.cpp
    #lasunzipper.cpp
    laswriteitemcompressed_v1.cpp
    laswriteitemcompressed_v2.cpp
    laswritepoint.cpp
    laszip.cpp
    laszip_dll.cpp
)

add_definitions(-DUNORDERED)
add_definitions(-DLASZIPDLL_EXPORTS)

set(LASZIP_SOURCES
  ${LASZIP_HPP}
  ${LASZIP_CPP}
  )

LASZIP_ADD_LIBRARY(${LASZIP_BASE_LIB_NAME} ${LASZIP_SOURCES})

set_target_properties(${LASZIP_BASE_LIB_NAME} PROPERTIES VERSION "${LASZIP_BUILD_VERSION}"
    SOVERSION "${LASZIP_API_VERSION}"
    CLEAN_DIRECT_OUTPUT 1)

