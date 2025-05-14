cmake_minimum_required(VERSION 3.8)

project(osmplot)

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SOURCES "src/*.cc")

add_executable(osmplot ${SOURCES})

target_include_directories(osmplot PRIVATE include)

message(STATUS "Looking for X11...")

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})
target_link_libraries(osmplot ${X11_LIBRARIES} )

set(CMAKE_BUILD_TYPE Debug)
