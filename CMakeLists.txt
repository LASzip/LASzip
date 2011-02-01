###############################################################################
# Main CMake configuration file for laszip
#
# Author: Mateusz Loskot <mateusz@loskot.net>
#
#
###############################################################################
# laszip general settings
project(laszip)


SET(LASZIP_VERSION_MAJOR "1")
SET(LASZIP_VERSION_MINOR "0")
SET(LASZIP_VERSION_PATCH "1")
set(VERSION "${LASZIP_VERSION_MAJOR}.${LASZIP_VERSION_MINOR}.${LASZIP_VERSION_PATCH}")
SET(LASZIP_LIB_SOVERSION "2.0.1")

# Name of C++ library
set(LASZIP_LIB_NAME laszip)

message(STATUS "Configuring LASzip library - ${LIBLAS_LIB_NAME}")

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
  if(NOT CMAKE_BUILD_TYPE)

  set(CMAKE_BUILD_TYPE Release CACHE STRING 
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel" FORCE)
  endif()
    message(STATUS "Setting LASzip build type - ${CMAKE_BUILD_TYPE}")
endif()

# TODO: Still testing the output paths --mloskot
set(LASZIP_BUILD_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})

# Output directory in which to build RUNTIME target files.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LASZIP_BUILD_OUTPUT_DIRECTORY})

# Output directory in which to build LIBRARY target files
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LASZIP_BUILD_OUTPUT_DIRECTORY})

# Output directory in which to build ARCHIVE target files.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LASZIP_BUILD_OUTPUT_DIRECTORY}) 


set(BUILD_STATIC FALSE CACHE BOOL "Build LASzip statically" )
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

###############################################################################
# Processing of project directories

add_subdirectory(src)

add_subdirectory(tools)


# Version information
SET(CPACK_PACKAGE_VERSION_MAJOR ${LASZIP_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${LASZIP_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${LASZIP_VERSION_MINOR})
SET(OSGEO4W_UPSTREAM_RELEASE "1")

SET(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
#SET(CPACK_RESOURCE_FILE_LICENSE "${laszip_SOURCE_DIR}/COPYING")

set(CPACK_SOURCE_PACKAGE_FILE_NAME
  "${CMAKE_PROJECT_NAME}-${VERSION}")

list(APPEND CPACK_SOURCE_IGNORE_FILES "_CPack_Packages")
list(APPEND CPACK_SOURCE_IGNORE_FILES ".gz")
list(APPEND CPACK_SOURCE_IGNORE_FILES "CMakeFiles")
list(APPEND CPACK_SOURCE_IGNORE_FILES "bin")
list(APPEND CPACK_SOURCE_IGNORE_FILES "hobu-config.bat")
list(APPEND CPACK_SOURCE_IGNORE_FILES "unused")
# list(APPEND CPACK_SOURCE_IGNORE_FILES ".zip")
list(APPEND CPACK_SOURCE_IGNORE_FILES ".hg")
# 
list(APPEND CPACK_SOURCE_IGNORE_FILES "CMakeCache.txt")
# 
# list(APPEND CPACK_SOURCE_IGNORE_FILES "CPackConfig.cmake")

include(CPack)

add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

#if (WIN32)
#    include(BuildOSGeo4W)
#endif(WIN32)
