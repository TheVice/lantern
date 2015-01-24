cmake_minimum_required(VERSION 2.8)

# Project name
project(lantern)

# Add testing tool
include(CTest)


# Additional compiler flags
# Compiler setup ============================
if (MSVC)
    set(COMPILER_FLAGS "${COMPILER_FLAGS} /W4 /GS")
    set(LINK_FLAGS "${LINK_FLAGS} /SafeSEH /DynamicBase /NXCompat")

    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(-DNOMINMAX)
else()
    set(COMPILER_FLAGS
    "-std=c++11 -flto -Wall -Werror -Wextra -Wno-comment -Wno-unknown-pragmas -Wno-unused-parameter")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEFAULT_CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")

if(CMAKE_HOST_WIN32)
    add_definitions(-DWIN32)
    add_definitions(-D_WIN32)
    add_definitions(-D_WINDOWS)
endif(CMAKE_HOST_WIN32)

if(NOT MSVC)
    add_definitions(-DGTEST_HAS_PTHREAD=0)
    add_definitions(-U__STRICT_ANSI__)
endif(NOT MSVC)

# ===========================================

# Library target ============================
set(LANTERN_SOURCES
    lantern/src/app.cpp
    lantern/src/camera.cpp
    lantern/src/color.cpp
    lantern/src/matrix3x3.cpp
    lantern/src/matrix4x4.cpp
    lantern/src/mesh.cpp
    lantern/src/obj_import.cpp
    lantern/src/pipeline.cpp
    lantern/src/texture.cpp
    lantern/src/vector3.cpp
    lantern/src/vector4.cpp)

set(LANTERN_HEADERS
    lantern/include/app.h
    lantern/include/camera.h
    lantern/include/color.h
    lantern/include/color_shader.h
    lantern/include/geometry_utils.h
    lantern/include/matrix3x3.h
    lantern/include/matrix4x4.h
    lantern/include/mesh.h
    lantern/include/mesh_attribute_info.h
    lantern/include/obj_import.h
    lantern/include/pipeline.h
    lantern/include/shader.h
    lantern/include/texture.h
    lantern/include/vector2.h
    lantern/include/vector3.h
    lantern/include/vector4.h)

add_library(
    lantern STATIC
    ${LANTERN_SOURCES}
    ${LANTERN_HEADERS})

target_include_directories(lantern PUBLIC lantern/include)

set_target_properties(
    lantern PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
# ===========================================

# Gooogle C++ testing framework target ======
include_directories(SYSTEM gtest-1.7.0/include)
include_directories(SYSTEM gtest-1.7.0)
# ===========================================

# Tests target ==============================
set(TESTS_SOURCES
	gtest-1.7.0/src/gtest-all.cc
    tests/src/camera.cpp
    tests/src/main.cpp
    tests/src/matrix3x3.cpp
    tests/src/matrix4x4.cpp
    tests/src/obj_import.cpp
    tests/src/pipeline.cpp
    tests/src/vector3.cpp
    tests/src/vector4.cpp)

set(TESTS_HEADERS
    tests/include/assert_utils.h)

add_executable(
    tests
    ${TESTS_SOURCES}
    ${TESTS_HEADERS}
    ${LANTERN_HEADERS})

target_include_directories(tests PRIVATE lantern/include tests/include)

set_target_properties(
    tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

target_link_libraries(tests lantern)

add_custom_command(
    TARGET tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/tests/resources"
    $<TARGET_FILE_DIR:tests>/resources)
# ===========================================

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

# Empty app target ==========================
add_executable(
    empty_app
    examples/empty_app/main.cpp)

set_target_properties(
    empty_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/empty_app")

target_include_directories(empty_app PRIVATE lantern/include)
target_link_libraries(empty_app lantern ${REQUIRED_LIBRARIES})
# ===========================================

# Rasterized triangle app target ======
add_executable(
    rasterized_triangle_app
    examples/rasterized_triangle_app/main.cpp)

set_target_properties(
    rasterized_triangle_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app")

target_include_directories(rasterized_triangle_app PRIVATE lantern/include)
target_link_libraries(rasterized_triangle_app lantern ${REQUIRED_LIBRARIES})

add_custom_command(
    TARGET rasterized_triangle_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/examples/rasterized_triangle_app/resources"
    $<TARGET_FILE_DIR:rasterized_triangle_app>/resources)
# ===========================================