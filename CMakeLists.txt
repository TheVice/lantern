cmake_minimum_required(VERSION 2.8.12)

# Project name
project(lantern)

# Add testing tool
include(CTest)

# SDL2 look up ==============================
if(NOT ANDROID)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
    find_package(SDL2 REQUIRED)
    find_package(SDL2IMAGE REQUIRED)
endif(NOT ANDROID)
# ===========================================

# Compiler setup ============================
if (MSVC)
    set(COMPILER_FLAGS
        "")

    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(-DNOMINMAX)
else()
    set(COMPILER_FLAGS
        "-std=gnu++0x -Wall -Wno-comment")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")
# ===========================================

# Android ===================================
if(ANDROID)
	string(REPLACE "\\" "/" PICO_PNG_SOURCE "$ENV{PICO_PNG_CPP}")

    set(ANDROID_EXECUTABLE android)
    set(ANT_EXECUTABLE ant)
    if(CMAKE_HOST_WIN32)
        set(ANDROID_EXECUTABLE android.bat)
        set(ANT_EXECUTABLE ant.bat)
    endif(CMAKE_HOST_WIN32)

    set(NDK_PATH $ENV{ANDROID_NDK})
    string(REPLACE "\\" "/" NDK_PATH ${NDK_PATH})
    string(TOLOWER ${CMAKE_BUILD_TYPE} ant_build_type)

    add_library(android_native_app_glue STATIC
                "${NDK_PATH}/sources/android/native_app_glue/android_native_app_glue.c")
    target_include_directories(android_native_app_glue SYSTEM PRIVATE "${NDK_PATH}/sources/android/native_app_glue")

    set_target_properties(
        android_native_app_glue PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${ANDROID_ABI}")
endif(ANDROID)
# ===========================================

# SDL2 and SDL2_image targets ===============
# if(ANDROID)
#     if(DEFINED ENV{SDL2SRCDIR})
#         string(REPLACE "\\" "/" SDL2_SRC_DIR $ENV{SDL2SRCDIR}/src)
#         string(REPLACE "\\" "/" SDL2_INCLUDE_DIR $ENV{SDL2SRCDIR}/include)
#     elseif(DEFINED SDL2SRCDIR)
#         string(REPLACE "\\" "/" SDL2_SRC_DIR ${SDL2SRCDIR}/src)
#         string(REPLACE "\\" "/" SDL2_INCLUDE_DIR ${SDL2SRCDIR}/include)
#     else()
#         message(FATAL_ERROR "Path to SDL2 source dir not defined")
#     endif()

#     if(DEFINED ENV{SDL2IMAGESRCDIR})
#         string(REPLACE "\\" "/" SDL2IMAGE_INCLUDE_DIR $ENV{SDL2IMAGESRCDIR})
#     elseif(DEFINED SDL2IMAGESRCDIR)
#         string(REPLACE "\\" "/" SDL2IMAGE_INCLUDE_DIR ${SDL2IMAGESRCDIR})
#     else()
#         message(FATAL_ERROR "Path to SDL2Image source dir not defined")
#     endif()

#     # SDL2
#     file(GLOB C_FILES ${SDL2_SRC_DIR}/*.c)
#     file(GLOB INC_FILES ${SDL2_INCLUDE_DIR}/*.h)

#     add_library(SDL2 STATIC ${INC_FILES} ${C_FILES})
#     target_include_directories(SDL2 PRIVATE SYSTEM ${SDL2_INCLUDE_DIR})
#     set(SDL2_LIBRARY SDL2)
#     set_target_properties(
#         SDL2 PROPERTIES
#         ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${ANDROID_ABI}")

#     # SDL2_image
#     file(GLOB C_FILES ${SDL2IMAGE_INCLUDE_DIR}/*.c)
#     file(GLOB INC_FILES ${SDL2IMAGE_INCLUDE_DIR}/*.h)

#     add_library(SDL2_image STATIC ${INC_FILES} ${C_FILES})
#     target_include_directories(SDL2_image PRIVATE SYSTEM ${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR})
#     set(SDL2IMAGE_LIBRARY SDL2_image)
#     set_target_properties(
#         SDL2_image PROPERTIES
#         ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${ANDROID_ABI}")
# endif(ANDROID)
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

if(NOT ANDROID)
	target_include_directories(lantern PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR})
    set_target_properties(
        lantern PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
else()
    set_target_properties(
        lantern PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${ANDROID_ABI}")
endif(NOT ANDROID)
# ===========================================

# Android app gate target ===================
if(ANDROID)
    file(GLOB CPP_FILES android/src/*.cpp)
    file(GLOB INC_FILES android/include/*.h)
    add_library(android_app_gate STATIC ${INC_FILES} ${CPP_FILES})
    target_include_directories(android_app_gate SYSTEM PRIVATE "${NDK_PATH}/sources/android/native_app_glue")
    target_include_directories(android_app_gate PRIVATE android/include ${LANTERN_INCLUDE_FOLDERS})

    set_target_properties(
        android_app_gate PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${ANDROID_ABI}")
endif(ANDROID)
# ===========================================

# Gooogle C++ testing framework target ======
if(DEFINED ENV{GTEST_ROOT})

    string(REPLACE "\\" "/" GTEST_SOURCES "$ENV{GTEST_ROOT}/src/gtest-all.cc")

    add_library(
        gtest STATIC
        ${GTEST_SOURCES})

    target_include_directories(gtest PRIVATE $ENV{GTEST_ROOT}/include $ENV{GTEST_ROOT})

    if(UNIX AND NOT ANDROID)
        target_link_libraries(gtest pthread)
    endif()

endif()
# ===========================================

# Android macros ============================
if(ANDROID)
    include(cmake/AndroidMacros.cmake)
endif(ANDROID)
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
if(NOT ANDROID)
    target_link_libraries(tests lantern ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY})
endif(NOT ANDROID)

add_custom_command(
    TARGET tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/tests/resources"
    $<TARGET_FILE_DIR:tests>/resources)

if(ANDROID)
    add_custom_target(run_tests
        DEPENDS tests
        COMMAND adb push ${CMAKE_BINARY_DIR}/tests /data/local/tmp
        COMMAND adb shell chmod 775 /data/local/tmp/tests
        COMMAND adb shell "cd /data/local/tmp ; /data/local/tmp/tests"
    )
endif(ANDROID)

if (WIN32)
    add_custom_command(
        TARGET tests
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL2_DLL}"
        $<TARGET_FILE_DIR:tests>)

    add_custom_command(
        TARGET tests
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL2IMAGE_DLL}"
        $<TARGET_FILE_DIR:tests>)

	add_custom_command(
        TARGET tests
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL2IMAGE_LIBPNG_DLL}"
        $<TARGET_FILE_DIR:tests>)

	add_custom_command(
        TARGET tests
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL2IMAGE_ZLIB_DLL}"
        $<TARGET_FILE_DIR:tests>)
endif()
# ===========================================

# Empty app target ==========================
if(ANDROID)
    file(GLOB target_source_files examples/empty_app/*.cpp)
    list(APPEND target_include_dirs android/include
                                    ${LANTERN_INCLUDE_FOLDERS}
                                    ${NDK_PATH}/sources/android/native_app_glue)
    list(APPEND target_link_libs android_app_gate
                                 android_native_app_glue
                                 android
                                 log
                                 EGL
                                 GLESv1_CM
                                 lantern)

    add_Android_Project(empty_app
                        EmptyAppNativeActivity
                        com.android.lantern.empty_app
                        ${CMAKE_BINARY_DIR}/examples/empty_app
                        "${target_source_files}"
                        "${target_include_dirs}"
                        "${target_link_libs}"
    )
else()
    add_executable(
        empty_app
        examples/empty_app/main.cpp)

    set_target_properties(
        empty_app PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/empty_app")

    target_include_directories(empty_app PRIVATE lantern/include)
    target_include_directories(empty_app PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})

    target_link_libraries(empty_app ${SDL2IMAGE_LIBRARY} ${SDL2_LIBRARY} lantern)

    if (WIN32)
        add_custom_command(
            TARGET empty_app
            POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
            "${SDL2_DLL}"
            $<TARGET_FILE_DIR:empty_app>)

        add_custom_command(
            TARGET empty_app
            POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
            "${SDL2IMAGE_DLL}"
            $<TARGET_FILE_DIR:empty_app>)

    	add_custom_command(
            TARGET empty_app
            POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
            "${SDL2IMAGE_LIBPNG_DLL}"
            $<TARGET_FILE_DIR:empty_app>)

    	add_custom_command(
            TARGET empty_app
            POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
            "${SDL2IMAGE_ZLIB_DLL}"
            $<TARGET_FILE_DIR:empty_app>)
    endif()
endif(ANDROID)
# ===========================================

# Rasterized triangle app target ======
if(ANDROID)
    file(GLOB target_source_files examples/rasterized_triangle_app/*.cpp)
    list(APPEND target_include_dirs android/include
                                    ${LANTERN_INCLUDE_FOLDERS}
                                    ${NDK_PATH}/sources/android/native_app_glue)
    list(APPEND target_link_libs android_app_gate
                                 android_native_app_glue
                                 android
                                 log
                                 EGL
                                 GLESv1_CM
                                 lantern)

    add_Android_Project(rasterized_triangle_app
                        RasterizedTriangleAppNativeActivity
                        com.android.lantern.rasterized_triangle_app
                        ${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app
                        "${target_source_files}"
                        "${target_include_dirs}"
                        "${target_link_libs}"
    )
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/examples/rasterized_triangle_app/resources" "${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app/assets/resources")
else()
    add_executable(
        rasterized_triangle_app
        examples/rasterized_triangle_app/main.cpp)

    set_target_properties(
        rasterized_triangle_app PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app")

    target_include_directories(rasterized_triangle_app PRIVATE lantern/include)
    target_include_directories(rasterized_triangle_app PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR})

    target_link_libraries(rasterized_triangle_app lantern ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY})

    if (WIN32)
        add_custom_command(
            TARGET rasterized_triangle_app
            POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
            "${SDL2_DLL}"
            $<TARGET_FILE_DIR:rasterized_triangle_app>)

        add_custom_command(
            TARGET rasterized_triangle_app
            POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
            "${SDL2IMAGE_DLL}"
            $<TARGET_FILE_DIR:rasterized_triangle_app>)

    	add_custom_command(
            TARGET rasterized_triangle_app
            POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
            "${SDL2IMAGE_LIBPNG_DLL}"
            $<TARGET_FILE_DIR:rasterized_triangle_app>)

    	add_custom_command(
            TARGET rasterized_triangle_app
            POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
            "${SDL2IMAGE_ZLIB_DLL}"
            $<TARGET_FILE_DIR:rasterized_triangle_app>)
    endif()

    add_custom_command(
        TARGET rasterized_triangle_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/examples/rasterized_triangle_app/resources"
        $<TARGET_FILE_DIR:rasterized_triangle_app>/resources)
endif(ANDROID)
# ===========================================