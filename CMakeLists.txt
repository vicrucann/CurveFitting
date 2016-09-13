cmake_minimum_required(VERSION 2.8.11)
project(curve_fit)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(OpenSceneGraph REQUIRED COMPONENTS osgViewer)

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS} )

set(SOURCES
    main-osg.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(libPathFitter)

target_link_libraries(${PROJECT_NAME}
    ${OPENSCENEGRAPH_LIBRARIES}
    libPathFitter
    )
