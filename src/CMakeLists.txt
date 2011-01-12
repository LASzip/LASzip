###############################################################################
#
# src/CMakeLists.txt controls building of laszip library
#
# Copyright (c) 2009 Mateusz Loskot <mateusz@loskot.net>
#
###############################################################################

set(LASZIP_HEADERS_DIR ../include/laszip)

set(LASZIP_HPP
  ${LASZIP_HEADERS_DIR}/laszipexport.hpp
  ${LASZIP_HEADERS_DIR}/laszip.hpp
  ${LASZIP_HEADERS_DIR}/laszipper.hpp
  ${LASZIP_HEADERS_DIR}/lasunzipper.hpp
  arithmeticdecoder.hpp
  arithmeticencoder.hpp
  arithmeticmodel.hpp
  bytestreamin.hpp
  bytestreamin_file.hpp
  bytestreamin_istream.hpp
  bytestreamout.hpp
  bytestreamout_file.hpp
  bytestreamout_ostream.hpp
  entropydecoder.hpp
  entropyencoder.hpp
  integercompressor.hpp
  lasreaditem.hpp
  lasreaditemcompressed_v1.hpp
  lasreaditemraw.hpp
  lasreadpoint.hpp
  laswriteitem.hpp
  laswriteitemcompressed_v1.hpp
  laswriteitemraw.hpp
  laswritepoint.hpp
  mydefs.hpp
 )

set(LASZIP_CPP
    arithmeticdecoder.cpp
    arithmeticencoder.cpp
    arithmeticmodel.cpp
    integercompressor.cpp
    lasreaditemcompressed_v1.cpp
    lasreadpoint.cpp
    lasunzipper.cpp
    laswriteitemcompressed_v1.cpp
    laswritepoint.cpp
    laszip.cpp
    laszipper.cpp
)

set(LASZIP_SOURCES
  ${LASZIP_HPP}
  ${LASZIP_CPP}
  )

if(WIN32)
    add_definitions("-DLASZIP_DLL_EXPORT=1")
endif()

# Group source files for IDE source explorers (e.g. Visual Studio)
source_group("CMake Files" FILES CMakeLists.txt)
source_group("Header Files" FILES ${LASZIP_HPP})
source_group("Source Files" FILES ${LASZIP_CPP})

# Standard include directory of laszip library
include_directories(../include ../include/laszip .)


add_library(${LASZIP_LIB_NAME} SHARED ${LASZIP_SOURCES})
set_target_properties(${LASZIP_LIB_NAME}
  PROPERTIES SOVERSION "${LASZIP_LIB_SOVERSION}" )

###############################################################################
# Targets installation

if (APPLE)
  set_target_properties(
    ${LASZIP_LIB_NAME}
    PROPERTIES
    INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
endif()

install(TARGETS ${LASZIP_LIB_NAME} 
  RUNTIME DESTINATION ${LASZIP_BIN_DIR}
  LIBRARY DESTINATION ${LASZIP_LIB_DIR}
  ARCHIVE DESTINATION ${LASZIP_LIB_DIR})

install(DIRECTORY ${LASZIP_HEADERS_DIR}
  DESTINATION ${LASZIP_INCLUDE_DIR}
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
