cmake_minimum_required(VERSION 2.8)

# Project name
project(lantern)

# Add testing tool
include(CTest)

include_directories(SYSTEM gtest-1.7.0/include)
include_directories(SYSTEM gtest-1.7.0)
add_definitions(-U__STRICT_ANSI__)

# Project directories
include_directories(
    examples/empty_app
    lantern/src lantern/include
    tests/src tests/include)

# Additional compiler flags
set(COMPILER_FLAGS
    "-std=c++11 -Wall -Werror")

# Add compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")

# Targets
#

# Library
#

set(LANTERN_SOURCES
    lantern/src/vector3.cpp
    lantern/src/vector4.cpp
    lantern/src/matrix3x3.cpp
    lantern/src/matrix4x4.cpp
    lantern/src/camera.cpp
    lantern/src/app.cpp
    lantern/src/bitmap_painter.cpp)
set(LANTERN_HEADERS
    lantern/include/vector3.h
    lantern/include/vector4.h
    lantern/include/matrix3x3.h
    lantern/include/matrix4x4.h
    lantern/include/camera.h
    lantern/include/app.h
    lantern/include/bitmap_painter.h)

add_library(
	lantern STATIC
	${LANTERN_SOURCES}
	${LANTERN_HEADERS})

# Tests
#

set(TESTS_SOURCES
    gtest-1.7.0/src/gtest-all.cc
    tests/src/main.cpp
    tests/src/vector3.cpp
    tests/src/vector4.cpp
    tests/src/matrix3x3.cpp
    tests/src/matrix4x4.cpp
    tests/src/camera.cpp)
set(TESTS_HEADERS
    tests/include/common.h)

add_executable(
	tests
	${TESTS_SOURCES}
	${TESTS_HEADERS}
	${LANTERN_HEADERS})

target_link_libraries(tests lantern pthread)

add_test(lantern_tests tests)

# Empty app example
#

set(REQUIRED_LIBRARIES)

if(CMAKE_HOST_WIN32)
  add_definitions(-DWIN32)
  add_definitions(-D_WIN32)
  add_definitions(-D_WINDOWS)

  list(APPEND REQUIRED_LIBRARIES glu32)
  list(APPEND REQUIRED_LIBRARIES opengl32)
endif(CMAKE_HOST_WIN32)

if(CMAKE_HOST_UNIX)
  list(APPEND REQUIRED_LIBRARIES GL)
  list(APPEND REQUIRED_LIBRARIES GLU)
  list(APPEND REQUIRED_LIBRARIES X11)
endif(CMAKE_HOST_UNIX)

add_executable(
    empty_app
    examples/empty_app/main.cpp
    ${LANTERN_HEADERS})

target_link_libraries(empty_app lantern ${REQUIRED_LIBRARIES})
