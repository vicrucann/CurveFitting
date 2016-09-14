cmake_minimum_required(VERSION 2.8.11)

if(POLICY CMP0020)
    CMAKE_POLICY(SET CMP0020 NEW)
endif()
if(POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

include_directories(${CMAKE_SOURCE_DIR})

set (libPathFitter_SRCS
    PathFitter.h
    PathFitter.cpp
    PathFitter-impl.cpp
    )

set (libPathFitterImpl_SRCS
    OsgPathFitter.h
    OsgPathFitter.cpp
    )

add_library(libPathFitter
    STATIC ${libPathFitter_SRCS}
    ${libPathFitterImpl_SRCS}
    )

target_link_libraries( libPathFitter
    ${OPENSCENEGRAPH_LIBRARIES}
    )
