#/**********************************************************\ 
# 
# Auto-generated CMakeLists.txt for the H3DPlugin project
#
#\**********************************************************/

# Written to work with cmake 2.6
cmake_minimum_required (VERSION 2.6)
set (CMAKE_BACKWARDS_COMPATIBILITY 2.6)

Project(${PLUGIN_NAME})

file (GLOB GENERAL RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    [^.]*.cpp
    [^.]*.h
    [^.]*.cmake
    )

include_directories(${PLUGIN_INCLUDE_DIRS})

# Generated files are stored in ${GENERATED} by the project configuration
SET_SOURCE_FILES_PROPERTIES(
    ${GENERATED}
    PROPERTIES
        GENERATED 1
    )

SOURCE_GROUP(Generated FILES
    ${GENERATED}
    )

SET( SOURCES
    ${GENERAL}
    ${GENERATED}
    )

# This will include Win/projectDef.cmake, X11/projectDef.cmake, Mac/projectDef 
# depending on the platform
include_platform()

# NF. Include additional libraries specific to this plugin

# Make sure we find all the H3DAPI CMake modules
SET(CMAKE_MODULE_PATH $ENV{H3D_ROOT}/build/modules )

# OpenGL
FIND_PACKAGE(OpenGL REQUIRED)

IF(OPENGL_FOUND)
  INCLUDE_DIRECTORIES( ${OPENGL_INCLUDE_DIR} ) 
  target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
ENDIF(OPENGL_FOUND)

# H3DAPI
FIND_PACKAGE(H3DAPI REQUIRED)

IF(H3DAPI_FOUND)
  INCLUDE_DIRECTORIES( ${H3DAPI_INCLUDE_DIR} ) 
  target_link_libraries(${PROJECT_NAME} ${H3DAPI_LIBRARIES})
ENDIF(H3DAPI_FOUND)

# HAPI
FIND_PACKAGE(HAPI REQUIRED)

IF(HAPI_FOUND)
  INCLUDE_DIRECTORIES( ${HAPI_INCLUDE_DIR} ) 
  target_link_libraries(${PROJECT_NAME} ${HAPI_LIBRARIES})
ENDIF(HAPI_FOUND)

# H3DUtil
FIND_PACKAGE(H3DUtil REQUIRED)

IF(H3DUTIL_FOUND)
  INCLUDE_DIRECTORIES( ${H3DUTIL_INCLUDE_DIR} ) 
  target_link_libraries(${PROJECT_NAME} ${H3DUTIL_LIBRARIES})
ENDIF(H3DUTIL_FOUND)