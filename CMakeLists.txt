cmake_minimum_required(VERSION 2.8)

# Project name
project(lantern)

# Add testing tool
include(CTest)

include_directories(SYSTEM gtest-1.7.0/include)
include_directories(SYSTEM gtest-1.7.0)
add_definitions(-U__STRICT_ANSI__)

# Project directories
include_directories(src include tests/src tests/include)

# Project files
#
set(LANTERN_SOURCES
    src/vector3.cpp
    src/vector4.cpp
    src/matrix3x3.cpp
    src/matrix4x4.cpp
    src/camera.cpp)
set(LANTERN_HEADERS
    include/vector3.h
    include/vector4.h
    include/matrix3x3.h
    include/matrix4x4.h
    include/camera.h)
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

# Add library target
add_library(
	lantern STATIC
	${LANTERN_SOURCES}
	${LANTERN_HEADERS})

# Add tests exec target
add_executable(
	tests
	${TESTS_SOURCES}
	${TESTS_HEADERS}
	${LANTERN_HEADERS})

# Link libraries to tests exec
target_link_libraries(tests lantern pthread)

# Add tests exec to tests list
add_test(lantern_tests tests)

# Additional compiler flags
set(COMPILER_FLAGS
    "-std=c++11 -Wall")

# Add compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")
