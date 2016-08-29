cmake_minimum_required(VERSION 2.8.11)
project(osgCurveFit)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui OpenGL)
find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgGA osgUtil osgViewer)

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

set(SOURCES
    mainCurveFit.cpp
    PathFitter.h
    PathFitter.cpp
    PathFitter-impl.cpp
    OsgPathFitter.h
    OsgPathFitter.cpp
    )

file(GLOB SHADER_SRC *.geom *.frag *.vert)

add_executable(${PROJECT_NAME} ${SHADER_SRC} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    ${OPENSCENEGRAPH_LIBRARIES}
    Qt5::Core
    Qt5::Gui
    Qt5::OpenGL
    )
