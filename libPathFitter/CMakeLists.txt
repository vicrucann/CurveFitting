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

if (OpenSceneGraph_FOUND) # or make an option to include your own class
    message("Using OSG data")
    set (libPathFitterImpl_SRCS
        OsgPathFitter.h
        OsgPathFitter.cpp
        )
    message("OpenSceneGraph-based data is used for curve fitting")
    add_definitions(-DUSE_OPENSCENEGRAPH=1)
endif()

add_library(libPathFitter
    STATIC ${libPathFitter_SRCS}
${libPathFitterImpl_SRCS}
)

target_link_libraries( libPathFitter
    ${OPENSCENEGRAPH_LIBRARIES}
)
