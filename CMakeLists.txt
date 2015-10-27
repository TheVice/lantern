cmake_minimum_required(VERSION 2.8.12)

# Project name
project(lantern)

# Add testing tool
include(CTest)

# SDL2, SDL2_image and FreeType look up ====
if(NOT ANDROID)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
    find_package(SDL2 REQUIRED)
    find_package(SDL2IMAGE REQUIRED)
    find_package(Freetype REQUIRED)
else()
    if(DEFINED ENV{SDL2SRCDIR})
        string(REPLACE "\\" "/" SDL2_SRC_DIR $ENV{SDL2SRCDIR}/src)
        string(REPLACE "\\" "/" SDL2_INCLUDE_DIR $ENV{SDL2SRCDIR}/include)
    elseif(DEFINED SDL2SRCDIR)
        string(REPLACE "\\" "/" SDL2_SRC_DIR ${SDL2SRCDIR}/src)
        string(REPLACE "\\" "/" SDL2_INCLUDE_DIR ${SDL2SRCDIR}/include)
    else()
        message(FATAL_ERROR "Path to SDL2 source dir not defined")
    endif()

    if(DEFINED ENV{SDL2IMAGESRCDIR})
        string(REPLACE "\\" "/" SDL2IMAGE_INCLUDE_DIR $ENV{SDL2IMAGESRCDIR})
    elseif(DEFINED SDL2IMAGESRCDIR)
        string(REPLACE "\\" "/" SDL2IMAGE_INCLUDE_DIR ${SDL2IMAGESRCDIR})
    else()
        message(FATAL_ERROR "Path to SDL2Image source dir not defined")
    endif()

    if(DEFINED ENV{FREETYPESRCDIR})
        string(REPLACE "\\" "/" FREETYPE_SRC_DIR $ENV{FREETYPESRCDIR}/src)
        string(REPLACE "\\" "/" FREETYPE_INCLUDE_DIRS $ENV{FREETYPESRCDIR}/include)
    elseif(DEFINED FREETYPESRCDIR)
        string(REPLACE "\\" "/" FREETYPE_SRC_DIR ${FREETYPESRCDIR}/src)
        string(REPLACE "\\" "/" FREETYPE_INCLUDE_DIRS ${FREETYPESRCDIR}/include)
    else()
        message(FATAL_ERROR "Path to FreeType source dir not defined")
    endif()
endif()
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
    if(CMAKE_HOST_WIN32)
        set(ANDROID_EXECUTABLE android.bat)
        set(ANT_EXECUTABLE ant.bat)
    else()
        set(ANDROID_EXECUTABLE android)
        set(ANT_EXECUTABLE ant)
    endif()

    string(REPLACE "\\" "/" NDK_PATH $ENV{ANDROID_NDK})
    string(TOLOWER ${CMAKE_BUILD_TYPE} ant_build_type)
endif()
# ===========================================

# SDL2, SDL2_image and FreeType targets =====
if(ANDROID)
    # SDL2
    file(GLOB C_FILES ${SDL2_SRC_DIR}/*.c
                      ${SDL2_SRC_DIR}/audio/*.c
                      ${SDL2_SRC_DIR}/audio/android/*.c
                      ${SDL2_SRC_DIR}/audio/dummy/*.c
                      ${SDL2_SRC_DIR}/atomic/SDL_atomic.c
                      ${SDL2_SRC_DIR}/atomic/SDL_spinlock.c
                      ${SDL2_SRC_DIR}/core/android/*.c
                      ${SDL2_SRC_DIR}/cpuinfo/*.c
                      ${SDL2_SRC_DIR}/dynapi/*.c
                      ${SDL2_SRC_DIR}/events/*.c
                      ${SDL2_SRC_DIR}/file/*.c
                      ${SDL2_SRC_DIR}/haptic/*.c
                      ${SDL2_SRC_DIR}/haptic/dummy/*.c
                      ${SDL2_SRC_DIR}/joystick/*.c
                      ${SDL2_SRC_DIR}/joystick/android/*.c
                      ${SDL2_SRC_DIR}/loadso/dlopen/*.c
                      ${SDL2_SRC_DIR}/power/*.c
                      ${SDL2_SRC_DIR}/power/android/*.c
                      ${SDL2_SRC_DIR}/filesystem/dummy/*.c
                      ${SDL2_SRC_DIR}/render/*.c
                      ${SDL2_SRC_DIR}/render/*/*.c
                      ${SDL2_SRC_DIR}/stdlib/*.c
                      ${SDL2_SRC_DIR}/thread/*.c
                      ${SDL2_SRC_DIR}/thread/pthread/*.c
                      ${SDL2_SRC_DIR}/timer/*.c
                      ${SDL2_SRC_DIR}/timer/unix/*.c
                      ${SDL2_SRC_DIR}/video/*.c
                      ${SDL2_SRC_DIR}/video/android/*.c
                      ${SDL2_SRC_DIR}/test/*.c
    )
  
    file(GLOB INC_FILES ${SDL2_INCLUDE_DIR}/*.h)

    add_library(SDL2 STATIC ${INC_FILES} ${C_FILES})
    target_include_directories(SDL2 PRIVATE SYSTEM ${SDL2_INCLUDE_DIR})

    add_definitions(-DGL_GLEXT_PROTOTYPES)

    set_target_properties(
        SDL2 PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${ANDROID_ABI}")
    set(SDL2_LIBRARY SDL2)

    # SDL2_image
    file(GLOB C_FILES ${SDL2IMAGE_INCLUDE_DIR}/*.c)
    file(GLOB INC_FILES ${SDL2IMAGE_INCLUDE_DIR}/*.h)

    list(APPEND C_FILES ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/png.c
                        ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/pngerror.c
                        ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/pngget.c
                        ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/pngmem.c
                        ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/pngpread.c
                        ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/pngread.c
                        ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/pngrio.c
                        ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/pngrtran.c
                        ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/pngrutil.c
                        ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/pngset.c
                        ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/pngtrans.c
                        ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/pngwio.c
                        ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/pngwrite.c
                        ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/pngwtran.c
                        ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/pngwutil.c
    )

    add_library(SDL2_image STATIC ${INC_FILES} ${C_FILES})
    target_include_directories(SDL2_image PRIVATE SYSTEM ${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR} 
                                                         ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2)

    add_definitions(-DLOAD_BMP)
    add_definitions(-DLOAD_GIF)
    add_definitions(-DLOAD_LBM)
    add_definitions(-DLOAD_PCX)
    add_definitions(-DLOAD_PNM)
    add_definitions(-DLOAD_TGA)
    add_definitions(-DLOAD_XCF)
    add_definitions(-DLOAD_XPM)
    add_definitions(-DLOAD_XV)
    add_definitions(-DLOAD_PNG)

    set_target_properties(
        SDL2_image PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${ANDROID_ABI}")
    set(SDL2IMAGE_LIBRARY SDL2_image)

    # FreeType
    set(C_FILES ${FREETYPE_SRC_DIR}/autofit/autofit.c
                ${FREETYPE_SRC_DIR}/base/ftadvanc.c
                ${FREETYPE_SRC_DIR}/base/ftbbox.c
                ${FREETYPE_SRC_DIR}/base/ftbdf.c
                ${FREETYPE_SRC_DIR}/base/ftbitmap.c
                ${FREETYPE_SRC_DIR}/base/ftcalc.c
                ${FREETYPE_SRC_DIR}/base/ftfntfmt.c
                ${FREETYPE_SRC_DIR}/base/ftfstype.c
                ${FREETYPE_SRC_DIR}/base/ftgasp.c
                ${FREETYPE_SRC_DIR}/base/ftgloadr.c
                ${FREETYPE_SRC_DIR}/base/ftglyph.c
                ${FREETYPE_SRC_DIR}/base/ftgxval.c
                ${FREETYPE_SRC_DIR}/base/ftinit.c
                ${FREETYPE_SRC_DIR}/base/ftlcdfil.c
                ${FREETYPE_SRC_DIR}/base/ftmm.c
                ${FREETYPE_SRC_DIR}/base/ftobjs.c
                ${FREETYPE_SRC_DIR}/base/ftotval.c
                ${FREETYPE_SRC_DIR}/base/ftoutln.c
                ${FREETYPE_SRC_DIR}/base/ftpatent.c
                ${FREETYPE_SRC_DIR}/base/ftpfr.c
                ${FREETYPE_SRC_DIR}/base/ftrfork.c
                ${FREETYPE_SRC_DIR}/base/ftstream.c
                ${FREETYPE_SRC_DIR}/base/ftstroke.c
                ${FREETYPE_SRC_DIR}/base/ftsynth.c
                ${FREETYPE_SRC_DIR}/base/ftsystem.c
                ${FREETYPE_SRC_DIR}/base/fttrigon.c
                ${FREETYPE_SRC_DIR}/base/fttype1.c
                ${FREETYPE_SRC_DIR}/base/ftutil.c
                ${FREETYPE_SRC_DIR}/base/ftwinfnt.c
                ${FREETYPE_SRC_DIR}/bdf/bdf.c
                ${FREETYPE_SRC_DIR}/cache/ftcache.c
                ${FREETYPE_SRC_DIR}/cff/cff.c
                ${FREETYPE_SRC_DIR}/cid/type1cid.c
                ${FREETYPE_SRC_DIR}/gzip/ftgzip.c
                ${FREETYPE_SRC_DIR}/gzip/zutil.c
                ${FREETYPE_SRC_DIR}/lzw/ftlzw.c
                ${FREETYPE_SRC_DIR}/pcf/pcf.c
                ${FREETYPE_SRC_DIR}/pfr/pfr.c
                ${FREETYPE_SRC_DIR}/psaux/psaux.c
                ${FREETYPE_SRC_DIR}/pshinter/pshinter.c
                ${FREETYPE_SRC_DIR}/psnames/psnames.c
                ${FREETYPE_SRC_DIR}/raster/raster.c
                ${FREETYPE_SRC_DIR}/sfnt/sfnt.c
                ${FREETYPE_SRC_DIR}/smooth/smooth.c
                ${FREETYPE_SRC_DIR}/truetype/truetype.c
                ${FREETYPE_SRC_DIR}/type1/type1.c
                ${FREETYPE_SRC_DIR}/type42/type42.c
                ${FREETYPE_SRC_DIR}/winfonts/winfnt.c
                ${FREETYPE_SRC_DIR}/bzip2/ftbzip2.c
                ${FREETYPE_SRC_DIR}/pcf/pcf.c
            ${SDL2IMAGE_INCLUDE_DIR}/external/zlib-1.2.8/zutil.c)

    add_library(FreeType STATIC ${C_FILES})
    target_include_directories(FreeType PRIVATE SYSTEM ${FREETYPE_INCLUDE_DIRS})
    add_definitions(-DFT2_BUILD_LIBRARY)
endif()
# ===========================================

# Library target ============================
file(GLOB LANTERN_HEADERS "lantern/include/*.h")

file(GLOB LANTERN_SOURCES "lantern/src/*.cpp")

set(LANTERN_INCLUDE_FOLDERS
    lantern/include
    lantern/include/math
    lantern/include/rendering
    lantern/include/rendering/shaders
    lantern/include/rendering/rasterizing
    lantern/include/rendering/geometry
    lantern/include/rendering/ui)

add_library(
    lantern STATIC
    ${LANTERN_SOURCES}
    ${LANTERN_HEADERS})

target_include_directories(lantern PUBLIC ${LANTERN_INCLUDE_FOLDERS})
target_include_directories(lantern PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})

if(NOT ANDROID)
    set_target_properties(
        lantern PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
else()
    set_target_properties(
        lantern PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${ANDROID_ABI}")
endif()
# ===========================================

# Gooogle C++ testing framework target ======
if(ANDROID)
    add_library(googletest_main STATIC 
                "${NDK_PATH}/sources/third_party/googletest/googletest/src/gtest-death-test.cc"
                "${NDK_PATH}/sources/third_party/googletest/googletest/src/gtest-filepath.cc"
                "${NDK_PATH}/sources/third_party/googletest/googletest/src/gtest-port.cc"
                "${NDK_PATH}/sources/third_party/googletest/googletest/src/gtest-printers.cc"
                "${NDK_PATH}/sources/third_party/googletest/googletest/src/gtest-test-part.cc"
                "${NDK_PATH}/sources/third_party/googletest/googletest/src/gtest-typed-test.cc"
                "${NDK_PATH}/sources/third_party/googletest/googletest/src/gtest.cc"
                "${NDK_PATH}/sources/third_party/googletest/googletest/src/gtest_main.cc")

    target_include_directories(googletest_main SYSTEM PRIVATE "$ENV{ANDROID_NDK}/sources/third_party/googletest/googletest/include")
    target_include_directories(googletest_main SYSTEM PRIVATE "$ENV{ANDROID_NDK}/sources/third_party/googletest/googletest")
else()
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
endif()
# ===========================================

# Android macros ============================
if(ANDROID)
    include(cmake/AndroidMacros.cmake)
endif()
# ===========================================

# Tests target ==============================
set(TESTS_SOURCES
    tests/src/camera.cpp
    tests/src/main.cpp
    tests/src/matrix3x3.cpp
    tests/src/matrix4x4.cpp
    tests/src/obj_import.cpp
    #tests/src/pipeline.cpp
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

if(NOT ANDROID)
    if (DEFINED ENV{GTEST_ROOT})
        target_include_directories(tests PRIVATE $ENV{GTEST_ROOT}/include)
    endif()
endif()

set_target_properties(
    tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

if(NOT ANDROID)
    target_link_libraries(tests lantern gtest ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY})
endif()

add_custom_command(
    TARGET tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/tests/resources"
    $<TARGET_FILE_DIR:tests>/resources)

if(ANDROID)
    target_include_directories(tests SYSTEM PRIVATE "$ENV{ANDROID_NDK}/sources/third_party/googletest/googletest/include")
    target_link_libraries(tests lantern googletest_main)
    add_custom_target(run_tests
        DEPENDS tests
        COMMAND adb push ${CMAKE_BINARY_DIR}/tests /data/local/tmp
        COMMAND adb shell chmod 775 /data/local/tmp/tests
        COMMAND adb shell "cd /data/local/tmp ; /data/local/tmp/tests")
endif()

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
    file(GLOB target_source_files examples/empty_app/*.cpp
                                  examples/empty_app/SDL_android_main.c)
    list(APPEND target_include_dirs ${LANTERN_INCLUDE_FOLDERS}
                                    ${SDL2_INCLUDE_DIR}
                                    ${SDL2_SRC_DIR}
                                    ${SDL2_IMAGE_INCLUDE_DIR}
                                    ${FREETYPE_INCLUDE_DIRS})
    list(APPEND target_link_libs lantern
                                 ${SDL2IMAGE_LIBRARY}
                                 ${SDL2_LIBRARY}
                                 FreeType
                                 GLESv1_CM
                                 GLESv2
                                 log
                                 android
                                 z)

    add_Android_Project(empty_app
                        EmptyAppNativeActivity
                        com.android.lantern.empty_app
                        ${CMAKE_BINARY_DIR}/examples/empty_app
                        "${target_source_files}"
                        "${target_include_dirs}"
                        "${target_link_libs}")
else()
    add_executable(
        empty_app WIN32
        examples/empty_app/main.cpp)

    set_target_properties(
        empty_app PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/empty_app")

    target_include_directories(empty_app PRIVATE lantern/include)
    target_include_directories(empty_app PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})

    target_link_libraries(empty_app lantern ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${FREETYPE_LIBRARIES})

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
endif()
# ===========================================

# Rasterized triangle app target ======
if(ANDROID)
    file(GLOB target_source_files examples/rasterized_triangle_app/*.cpp
                                  examples/rasterized_triangle_app/SDL_android_main.c)
    list(APPEND target_include_dirs ${LANTERN_INCLUDE_FOLDERS}
                                    ${SDL2_INCLUDE_DIR}
                                    ${SDL2_SRC_DIR}
                                    ${SDL2_IMAGE_INCLUDE_DIR}
                                    ${FREETYPE_INCLUDE_DIRS})
    list(APPEND target_link_libs lantern
                                 ${SDL2IMAGE_LIBRARY}
                                 ${SDL2_LIBRARY}
                                 FreeType
                                 GLESv1_CM
                                 GLESv2
                                 log
                                 android
                                 z)

    add_Android_Project(rasterized_triangle_app
                        RasterizedTriangleAppNativeActivity
                        com.android.lantern.rasterized_triangle_app
                        ${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app
                        "${target_source_files}"
                        "${target_include_dirs}"
                        "${target_link_libs}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/examples/rasterized_triangle_app/resources" "${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app/assets/resources")
else()
    add_executable(
        rasterized_triangle_app WIN32
        examples/rasterized_triangle_app/main.cpp)

    set_target_properties(
        rasterized_triangle_app PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app")

    target_include_directories(rasterized_triangle_app PRIVATE lantern/include)
    target_include_directories(rasterized_triangle_app PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})

    target_link_libraries(rasterized_triangle_app lantern ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${FREETYPE_LIBRARIES})

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
endif()
# ===========================================