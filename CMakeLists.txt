###############################################################################
# Main CMake configuration file for laszip
#
# Author: Mateusz Loskot <mateusz@loskot.net>
#
#
###############################################################################
# laszip general settings
project(laszip)

# Name of C++ library
set(LASZIP_LIB_NAME liblaszip)


set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

###############################################################################
# CMake settings
cmake_minimum_required(VERSION 2.6.0)

set(CMAKE_COLOR_MAKEFILE ON)

# Allow advanced users to generate Makefiles printing detailed commands
mark_as_advanced(CLEAR CMAKE_VERBOSE_MAKEFILE )

# Path to additional CMake modules
set(CMAKE_MODULE_PATH ${laszip_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

###############################################################################
# General build settings

# Default to release mode 
if(NOT MSVC_IDE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING 
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel")
endif()

# TODO: Still testing the output paths --mloskot
set(LASZIP_BUILD_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})

# Output directory in which to build RUNTIME target files.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LASZIP_BUILD_OUTPUT_DIRECTORY})

# Output directory in which to build LIBRARY target files
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LASZIP_BUILD_OUTPUT_DIRECTORY})

# Output directory in which to build ARCHIVE target files.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LASZIP_BUILD_OUTPUT_DIRECTORY}) 

###############################################################################
# Platform and compiler specific settings

if(WIN32)
  if (MSVC)
    if (MSVC80 OR MSVC90 OR MSVC10)
      add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
      add_definitions(-D_CRT_NONSTDC_NO_WARNING)
      add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    endif()
  endif()
endif()

###############################################################################
# Installation settings

if(WIN32)
  set(DEFAULT_LIB_SUBDIR lib)
  set(DEFAULT_DATA_SUBDIR .)
  set(DEFAULT_INCLUDE_SUBDIR include)

  if (MSVC)
    set(DEFAULT_BIN_SUBDIR bin)
  else()
    set(DEFAULT_BIN_SUBDIR .)
  endif()
else()
  # Common locations for Unix and Mac OS X
  set(DEFAULT_BIN_SUBDIR bin)
  set(DEFAULT_LIB_SUBDIR lib)
  set(DEFAULT_INCLUDE_SUBDIR include)
endif()

# Locations are changeable by user to customize layout of laszip installation
# (default values are platform-specific)
set(LASZIP_BIN_SUBDIR ${DEFAULT_BIN_SUBDIR} CACHE STRING
  "Subdirectory where executables will be installed")
set(LASZIP_LIB_SUBDIR ${DEFAULT_LIB_SUBDIR} CACHE STRING
  "Subdirectory where libraries will be installed")
set(LASZIP_INCLUDE_SUBDIR ${DEFAULT_INCLUDE_SUBDIR} CACHE STRING
  "Subdirectory where header files will be installed")
set(LASZIP_DATA_SUBDIR ${DEFAULT_DATA_SUBDIR} CACHE STRING
  "Subdirectory where data will be installed")

# Mark *_SUBDIR variables as advanced and dedicated to use by power-users only.
mark_as_advanced(LASZIP_BIN_SUBDIR
  LASZIP_LIB_SUBDIR LASZIP_INCLUDE_SUBDIR LASZIP_DATA_SUBDIR)

# Full paths for the installation
set(LASZIP_BIN_DIR ${LASZIP_BIN_SUBDIR})
set(LASZIP_LIB_DIR ${LASZIP_LIB_SUBDIR})
set(LASZIP_INCLUDE_DIR ${LASZIP_INCLUDE_SUBDIR})
set(LASZIP_DATA_DIR ${LASZIP_DATA_SUBDIR})

###############################################################################
# Processing of project directories

add_subdirectory(src)

add_subdirectory(tools)