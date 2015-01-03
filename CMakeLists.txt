cmake_minimum_required(VERSION 2.8)

# Project name
project(lantern)

# Add testing tool
include(CTest)

include_directories(SYSTEM gtest-1.7.0/include)
include_directories(SYSTEM gtest-1.7.0)

# Project directories
include_directories(
    examples/empty_app
    examples/rotating_car_app
    lantern/src lantern/include
    tests/src tests/include)

# Additional compiler flags
if (MSVC)
    set(COMPILER_FLAGS "${COMPILER_FLAGS} /W4 /GS")
    set(LINK_FLAGS "${LINK_FLAGS} /SafeSEH /DynamicBase /NXCompat")
else()
    set(COMPILER_FLAGS
    "-std=c++11 -flto -Wall -Werror -Wextra -Wno-comment -Wno-unknown-pragmas -Wno-unused-parameter")
endif()

# Add compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEFAULT_CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")

# Additional MSVC-related stuff
#
if (MSVC)
    # Setup dependencies folder
    #
    set(MSVC_DEPENDENCIES_FOLDER
        "${CMAKE_BINARY_DIR}/dependencies/")
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${MSVC_DEPENDENCIES_FOLDER}/include/)
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${MSVC_DEPENDENCIES_FOLDER}/lib/)
    include_directories(${MSVC_DEPENDENCIES_FOLDER}/include/)
    link_directories(${MSVC_DEPENDENCIES_FOLDER}/lib/)

    # Add math defines
    add_definitions(-D_USE_MATH_DEFINES)
endif()

if(CMAKE_HOST_WIN32)
    add_definitions(-DWIN32)
    add_definitions(-D_WIN32)
    add_definitions(-D_WINDOWS)
endif(CMAKE_HOST_WIN32)

if(NOT MSVC)
    add_definitions(-DGTEST_HAS_PTHREAD=0)
    add_definitions(-U__STRICT_ANSI__)
endif(NOT MSVC)

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
    lantern/src/point.cpp
    lantern/src/color.cpp
    lantern/src/bitmap_painter.cpp
    lantern/src/mesh.cpp
    lantern/src/obj_import.cpp
    lantern/src/scene_object.cpp
    lantern/src/scene.cpp
    lantern/src/pipeline.cpp)
set(LANTERN_HEADERS
    lantern/include/vector3.h
    lantern/include/vector4.h
    lantern/include/matrix3x3.h
    lantern/include/matrix4x4.h
    lantern/include/point.h
    lantern/include/color.h
    lantern/include/camera.h
    lantern/include/app.h
    lantern/include/bitmap_painter.h
    lantern/include/mesh.h
    lantern/include/obj_import.h
    lantern/include/scene_object.h
    lantern/include/scene.h
    lantern/include/pipeline.h)

add_library(
	lantern STATIC
	${LANTERN_SOURCES}
	${LANTERN_HEADERS})

set_target_properties(
    lantern PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Tests
#

set(TESTS_SOURCES
    gtest-1.7.0/src/gtest-all.cc
    tests/src/main.cpp
    tests/src/vector3.cpp
    tests/src/vector4.cpp
    tests/src/matrix3x3.cpp
    tests/src/matrix4x4.cpp
    tests/src/camera.cpp
    tests/src/bitmap_painter.cpp
    tests/src/obj_import.cpp)
set(TESTS_HEADERS
    tests/include/common.h)

add_executable(
	tests
	${TESTS_SOURCES}
	${TESTS_HEADERS}
	${LANTERN_HEADERS})

set_target_properties(
    tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/")

add_custom_command(
    TARGET tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/tests/resources"
    $<TARGET_FILE_DIR:tests>/resources)

target_link_libraries(tests lantern)

add_test(lantern_tests tests)

# Empty app example
#

set(REQUIRED_LIBRARIES)

if(CMAKE_HOST_WIN32)
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

set_target_properties(
    empty_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/empty_app")

if(APPLE)
    MESSAGE("Unable to test on Mac OS X")
    # target_link_libraries(empty_app lantern /Library/Frameworks/SDL2.framework)
else()
    target_link_libraries(empty_app lantern ${REQUIRED_LIBRARIES})
endif(APPLE)

# Rotating car app example
#

add_executable(
    rotating_car_app
    examples/rotating_car_app/main.cpp
    ${LANTERN_HEADERS})

set_target_properties(
    rotating_car_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/rotating_car_app")

add_custom_command(
    TARGET rotating_car_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/examples/rotating_car_app/resources"
    $<TARGET_FILE_DIR:rotating_car_app>/resources)

if(APPLE)
    MESSAGE("Unable to test on Mac OS X")
    # target_link_libraries(rotating_car_app lantern /Library/Frameworks/SDL2.framework)
else()
    target_link_libraries(rotating_car_app lantern ${REQUIRED_LIBRARIES})
endif(APPLE)
