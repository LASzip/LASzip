###############################################################################
#
# src/CMakeLists.txt controls building of laszip library
#
# Copyright (c) 2009 Mateusz Loskot <mateusz@loskot.net>
#
###############################################################################

set(LASZIP_HEADERS_DIR ../include/laszip)

set(LASZIP_HPP
  ${LASZIP_HEADERS_DIR}/lasdefinitions.hpp
  ${LASZIP_HEADERS_DIR}/laspointreader.hpp
  ${LASZIP_HEADERS_DIR}/laspointwriter.hpp
  ${LASZIP_HEADERS_DIR}/lasdefinitions.hpp
  ${LASZIP_HEADERS_DIR}/laspointwriter0compressed.hpp
  ${LASZIP_HEADERS_DIR}/laspointreader.hpp
  ${LASZIP_HEADERS_DIR}/laspointwriter0compressedarithmetic.hpp
  ${LASZIP_HEADERS_DIR}/laspointreader0compressed.hpp
  ${LASZIP_HEADERS_DIR}/laspointwriter0raw.hpp
  ${LASZIP_HEADERS_DIR}/laspointreader0compressedarithmetic.hpp
  ${LASZIP_HEADERS_DIR}/laspointwriter1compressed.hpp
  ${LASZIP_HEADERS_DIR}/laspointreader0raw.hpp
  ${LASZIP_HEADERS_DIR}/laspointwriter1compressedarithmetic.hpp
  ${LASZIP_HEADERS_DIR}/laspointreader1compressed.hpp
  ${LASZIP_HEADERS_DIR}/laspointwriter1raw.hpp
  ${LASZIP_HEADERS_DIR}/laspointreader1compressedarithmetic.hpp
  ${LASZIP_HEADERS_DIR}/laspointwriter2compressed.hpp
  ${LASZIP_HEADERS_DIR}/laspointreader1raw.hpp
  ${LASZIP_HEADERS_DIR}/laspointwriter2compressedarithmetic.hpp
  ${LASZIP_HEADERS_DIR}/laspointreader2compressed.hpp
  ${LASZIP_HEADERS_DIR}/laspointwriter2raw.hpp
  ${LASZIP_HEADERS_DIR}/laspointreader2compressedarithmetic.hpp
  ${LASZIP_HEADERS_DIR}/laspointwriter3compressed.hpp
  ${LASZIP_HEADERS_DIR}/laspointreader2raw.hpp
  ${LASZIP_HEADERS_DIR}/laspointwriter3compressedarithmetic.hpp
  ${LASZIP_HEADERS_DIR}/laspointreader3compressed.hpp
  ${LASZIP_HEADERS_DIR}/laspointwriter3raw.hpp
  ${LASZIP_HEADERS_DIR}/laspointreader3compressedarithmetic.hpp
  ${LASZIP_HEADERS_DIR}/lasreader.hpp
  ${LASZIP_HEADERS_DIR}/laspointreader3raw.hpp
  ${LASZIP_HEADERS_DIR}/laswriter.hpp
  ${LASZIP_HEADERS_DIR}/liblaszip.hpp
)

set(LASZIP_CPP
    arithmeticdecoder.cpp
    laspointwriter0compressedarithmetic.cpp
    arithmeticencoder.cpp
    laspointwriter1compressed.cpp
    arithmeticintegercompressor.cpp
    laspointwriter1compressedarithmetic.cpp
    arithmeticmodel.cpp
    laspointwriter2compressed.cpp
    laspointreader0compressed.cpp
    laspointwriter2compressedarithmetic.cpp
    laspointreader0compressedarithmetic.cpp
    laspointwriter3compressed.cpp
    laspointreader1compressed.cpp
    laspointwriter3compressedarithmetic.cpp
    laspointreader1compressedarithmetic.cpp
    lasreader.cpp
    laspointreader2compressed.cpp
    laswriter.cpp
    laspointreader2compressedarithmetic.cpp
    rangedecoder.cpp
    laspointreader3compressed.cpp
    rangeencoder.cpp
    laspointreader3compressedarithmetic.cpp
    rangeintegercompressor.cpp
    laspointwriter0compressed.cpp
    rangemodel.cpp
    liblaszip.cpp
)

set(LASZIP_SOURCES
  ${LASZIP_HPP}
  ${LASZIP_CPP}
  )
# Group source files for IDE source explorers (e.g. Visual Studio)
source_group("CMake Files" FILES CMakeLists.txt)
source_group("Header Files" FILES ${LASZIP_HPP})
source_group("Source Files" FILES ${LASZIP_CPP})

# Standard include directory of laszip library
include_directories(../include ../include/laszip .)

add_library(${LASZIP_LIB_NAME} STATIC ${LASZIP_SOURCES})

###############################################################################
# Targets installation

install(TARGETS ${LASZIP_LIB_NAME} 
  RUNTIME DESTINATION ${LASZIP_BIN_DIR}
  LIBRARY DESTINATION ${LASZIP_BIN_DIR}
  ARCHIVE DESTINATION ${LASZIP_BIN_DIR})

install(DIRECTORY ${LASZIP_HEADERS_DIR}
  DESTINATION ${LASZIP_INCLUDE_DIR}
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")