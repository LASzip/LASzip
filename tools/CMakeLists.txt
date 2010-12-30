###############################################################################
#
# tools/CMakeLists.txt controls building of laszip utilities 
#
# Copyright (c) 2009 Mateusz Loskot <mateusz@loskot.net>
#
###############################################################################

include_directories(
    .
    ../include
    ../include/laszip)


set(APPS_CPP_DEPENDENCIES
    ${LASZIP_LIB_NAME})
    

#set(LASZIP laszip)
#set(LASDIFF lasdiff)
set(LASZIPPERTEST laszippertest)


set(LASZIP_UTILITIES
    ${LASZIP} ${LASDIFF} ${LASZIPPERTEST}
    )
    
if(WIN32)
    add_definitions("-DLASZIP_DLL_EXPORT=1")
endif()


# TODO: Experimental and requires testing --mloskot
# Generate user-specific settings for Visual Studio project
set(VCPROJ_USER_REMOTE_MACHINE_DEBUG ${MACHINE_NAME})
set(VCPROJ_USER_ENVIRONMENT_DEBUG "${ENVIRONMENT_PATH}")

if(MSVC)
    foreach(utility ${LASZIP_UTILITIES})
        set(USER_FILE ${utility}.vcproj.$ENV{USERDOMAIN}.$ENV{USERNAME}.user)
        set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE})
        message(STATUS "Generating ${CMAKE_GENERATOR} user-specific settings in ${USER_FILE}")
        configure_file(${CMAKE_SOURCE_DIR}/cmake/laszip.vcproj.user.template ${OUTPUT_PATH} @ONLY)
    endforeach()
endif()

if(LASZIP)
    add_executable(${LASZIP} laszip.cpp)
    target_link_libraries(${LASZIP} ${APPS_CPP_DEPENDENCIES} )
endif()

if(LASDIFF)
    add_executable(${LASDIFF} lasdiff.cpp)
    target_link_libraries(${LASDIFF} ${APPS_CPP_DEPENDENCIES} )
endif()

if(LASZIPPERTEST)
    add_executable(${LASZIPPERTEST} laszippertest.cpp)
    target_link_libraries(${LASZIPPERTEST} ${APPS_CPP_DEPENDENCIES} )
endif()

###############################################################################
# Targets installation

install(TARGETS ${LASZIP_UTILITIES}
    RUNTIME DESTINATION ${LASZIP_BIN_DIR}
    LIBRARY DESTINATION ${LASZIP_LIB_DIR}
    ARCHIVE DESTINATION ${LASZIP_LIB_DIR})
