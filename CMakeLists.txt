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
        "-std=gnu++0x -Wall -Wextra -Wno-comment -Wno-unknown-pragmas -Wno-unused-parameter")
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

string(REPLACE "\\" "/" PICO_PNG_SOURCE "$ENV{PICO_PNG_CPP}")

# ===========================================

# Library target ============================
set(LANTERN_SOURCES
    lantern/src/app.cpp
    lantern/src/math/line.cpp
    lantern/src/math/math_common.cpp
    lantern/src/math/matrix3x3.cpp
    lantern/src/math/matrix4x4.cpp
    lantern/src/math/vector4.cpp
    lantern/src/rendering/camera.cpp
    lantern/src/rendering/color.cpp
    lantern/src/rendering/pipeline.cpp
    lantern/src/rendering/texture.cpp
    lantern/src/rendering/geometry/mesh.cpp
    lantern/src/rendering/geometry/obj_import.cpp
    lantern/src/rendering/rasterizing/rasterizer_traversal_common.cpp
	${PICO_PNG_SOURCE})

set(LANTERN_HEADERS
    lantern/include/app.h
    lantern/include/math/aabb.h
    lantern/include/math/line.h
    lantern/include/math/math_common.h
    lantern/include/math/matrix3x3.h
    lantern/include/math/matrix4x4.h
    lantern/include/math/vector2.h
    lantern/include/math/vector3.h
    lantern/include/math/vector4.h
    lantern/include/rendering/texture.h
    lantern/include/rendering/rendering_options.h
    lantern/include/rendering/pipeline.h
    lantern/include/rendering/color.h
    lantern/include/rendering/camera.h
    lantern/include/rendering/shaders/shader.h
    lantern/include/rendering/shaders/color_shader.h
    lantern/include/rendering/shaders/texture_shader.h
    lantern/include/rendering/rasterizing/rasterizer_common.h
    lantern/include/rendering/rasterizing/rasterizer_homogeneous.h
    lantern/include/rendering/rasterizing/rasterizer_inversed_slope.h
    lantern/include/rendering/rasterizing/rasterizer_traversal_aabb.h
    lantern/include/rendering/rasterizing/rasterizer_traversal_backtracking.h
    lantern/include/rendering/rasterizing/rasterizer_traversal_common.h
    lantern/include/rendering/rasterizing/rasterizer_traversal_zigzag.h
    lantern/include/rendering/geometry/obj_import.h
    lantern/include/rendering/geometry/mesh_attribute_info.h
    lantern/include/rendering/geometry/mesh.h)

set(LANTERN_INCLUDE_FOLDERS
    lantern/include
    lantern/include/math
    lantern/include/rendering
    lantern/include/rendering/shaders
    lantern/include/rendering/rasterizing
    lantern/include/rendering/geometry)

add_library(
    lantern STATIC
    ${LANTERN_SOURCES}
    ${LANTERN_HEADERS})

target_include_directories(lantern PUBLIC ${LANTERN_INCLUDE_FOLDERS})

set_target_properties(
    lantern PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
# ===========================================

# Gooogle C++ testing framework target ======
if(DEFINED ENV{GTEST_ROOT})

    string(REPLACE "\\" "/" GTEST_SOURCES "$ENV{GTEST_ROOT}/src/gtest-all.cc")

    add_library(
        gtest STATIC
        ${GTEST_SOURCES})

    target_include_directories(gtest PRIVATE $ENV{GTEST_ROOT}/include $ENV{GTEST_ROOT})

    if(UNIX)
        target_link_libraries(gtest pthread)
    endif()

endif()
# ===========================================

# Tests target ==============================
set(TESTS_SOURCES
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

if (DEFINED ENV{GTEST_ROOT})
    target_include_directories(tests PRIVATE $ENV{GTEST_ROOT}/include)
endif()

set_target_properties(
    tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

target_link_libraries(tests lantern gtest)

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