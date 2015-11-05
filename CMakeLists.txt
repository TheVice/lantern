cmake_minimum_required(VERSION 2.8.12)

# Project name
project(lantern)

# Add testing tool
include(CTest)

set(SDL2FROMSOURCE 0)
set(SDL2IMAGEFROMSOURCE 0)
set(FREETYPEFROMSOURCE 0)

if(CMAKE_HOST_WIN32)
if(DEFINED ENV{SDL2DIR})
    string(REPLACE "\\" "/" SDL2_SRC_DIR $ENV{SDL2DIR}/src)
    if(EXISTS ${SDL2_SRC_DIR}/SDL.c)
        string(REPLACE "\\" "/" SDL2_INCLUDE_DIR $ENV{SDL2DIR}/include)
        set(SDL2FROMSOURCE 1)
    endif()
elseif(DEFINED SDL2DIR)
    string(REPLACE "\\" "/" SDL2_SRC_DIR ${SDL2DIR}/src)
    if(EXISTS ${SDL2_SRC_DIR}/SDL.c)
        string(REPLACE "\\" "/" SDL2_INCLUDE_DIR ${SDL2DIR}/include)
        set(SDL2FROMSOURCE 1)
    endif()
endif()

if(DEFINED ENV{SDL2IMAGEDIR})
    string(REPLACE "\\" "/" SDL2IMAGE_INCLUDE_DIR $ENV{SDL2IMAGEDIR})
    if(EXISTS ${SDL2IMAGE_INCLUDE_DIR}/IMG.c)
        set(SDL2IMAGEFROMSOURCE 1)
    endif()
elseif(DEFINED SDL2IMAGEDIR)
    string(REPLACE "\\" "/" SDL2IMAGE_INCLUDE_DIR ${SDL2IMAGEDIR})
    if(EXISTS ${SDL2IMAGE_INCLUDE_DIR}/IMG.c)
        set(SDL2IMAGEFROMSOURCE 1)
    endif()
endif()

if(DEFINED ENV{FREETYPEDIR})
    string(REPLACE "\\" "/" FREETYPE_SRC_DIR $ENV{FREETYPEDIR}/src)
    if(EXISTS ${FREETYPE_SRC_DIR}/base/ftapi.c)
        string(REPLACE "\\" "/" FREETYPE_INCLUDE_DIRS $ENV{FREETYPEDIR}/include)
        set(FREETYPEFROMSOURCE 1)
    endif()
    string(REPLACE "\\" "/" FREETYPEDIR $ENV{FREETYPEDIR})
elseif(DEFINED FREETYPEDIR)
    string(REPLACE "\\" "/" FREETYPE_SRC_DIR ${FREETYPEDIR}/src)
    if(EXISTS ${FREETYPE_SRC_DIR}/base/ftapi.c)
        string(REPLACE "\\" "/" FREETYPE_INCLUDE_DIRS ${FREETYPEDIR}/include)
        set(FREETYPEFROMSOURCE 1)
    endif()
    string(REPLACE "\\" "/" FREETYPEDIR ${FREETYPEDIR})
# else()
#     if(NOT UNIX)
#         message(FATAL_ERROR "Path to FreeType source or binary directory not set or not defined")
#     endif()
endif()
endif()

# SDL2, SDL2_image and FreeType look up =====
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
if(SDL2FROMSOURCE)
    message("SDL2 will making from source")
    if(CMAKE_HOST_WIN32)
        # SDL2
        set(SDL2_SRC_FILES ${SDL2_SRC_DIR}/atomic/SDL_atomic.c
                           ${SDL2_SRC_DIR}/atomic/SDL_spinlock.c
                           ${SDL2_SRC_DIR}/audio/SDL_audio.c
                           ${SDL2_SRC_DIR}/audio/SDL_audiocvt.c
                           ${SDL2_SRC_DIR}/audio/SDL_audiodev.c
                           ${SDL2_SRC_DIR}/audio/SDL_audiotypecvt.c
                           ${SDL2_SRC_DIR}/audio/SDL_mixer.c
                           ${SDL2_SRC_DIR}/audio/SDL_wave.c
                           ${SDL2_SRC_DIR}/core/windows/SDL_windows.c
                           ${SDL2_SRC_DIR}/cpuinfo/SDL_cpuinfo.c
                           ${SDL2_SRC_DIR}/dynapi/SDL_dynapi.c
                           ${SDL2_SRC_DIR}/events/SDL_clipboardevents.c
                           ${SDL2_SRC_DIR}/events/SDL_dropevents.c
                           ${SDL2_SRC_DIR}/events/SDL_events.c
                           ${SDL2_SRC_DIR}/events/SDL_gesture.c
                           ${SDL2_SRC_DIR}/events/SDL_keyboard.c
                           ${SDL2_SRC_DIR}/events/SDL_mouse.c
                           ${SDL2_SRC_DIR}/events/SDL_quit.c
                           ${SDL2_SRC_DIR}/events/SDL_touch.c
                           ${SDL2_SRC_DIR}/events/SDL_windowevents.c
                           ${SDL2_SRC_DIR}/filesystem/windows/SDL_sysfilesystem.c
                           ${SDL2_SRC_DIR}/file/SDL_rwops.c
                           ${SDL2_SRC_DIR}/haptic/SDL_haptic.c
                           ${SDL2_SRC_DIR}/haptic/windows/SDL_syshaptic.c
                           ${SDL2_SRC_DIR}/joystick/SDL_gamecontroller.c
                           ${SDL2_SRC_DIR}/joystick/SDL_joystick.c
                           ${SDL2_SRC_DIR}/joystick/windows/SDL_dxjoystick.c
                           ${SDL2_SRC_DIR}/joystick/windows/SDL_mmjoystick.c
                           ${SDL2_SRC_DIR}/libm/e_atan2.c
                           ${SDL2_SRC_DIR}/libm/e_log.c
                           ${SDL2_SRC_DIR}/libm/e_pow.c
                           ${SDL2_SRC_DIR}/libm/e_rem_pio2.c
                           ${SDL2_SRC_DIR}/libm/e_sqrt.c
                           ${SDL2_SRC_DIR}/libm/k_cos.c
                           ${SDL2_SRC_DIR}/libm/k_rem_pio2.c
                           ${SDL2_SRC_DIR}/libm/k_sin.c
                           ${SDL2_SRC_DIR}/libm/s_atan.c
                           ${SDL2_SRC_DIR}/libm/s_copysign.c
                           ${SDL2_SRC_DIR}/libm/s_cos.c
                           ${SDL2_SRC_DIR}/libm/s_fabs.c
                           ${SDL2_SRC_DIR}/libm/s_floor.c
                           ${SDL2_SRC_DIR}/libm/s_scalbn.c
                           ${SDL2_SRC_DIR}/libm/s_sin.c
                           ${SDL2_SRC_DIR}/loadso/windows/SDL_sysloadso.c
                           ${SDL2_SRC_DIR}/power/SDL_power.c
                           ${SDL2_SRC_DIR}/power/windows/SDL_syspower.c
                           ${SDL2_SRC_DIR}/render/direct3d/SDL_render_d3d.c
                           ${SDL2_SRC_DIR}/render/opengl/SDL_render_gl.c
                           ${SDL2_SRC_DIR}/render/opengl/SDL_shaders_gl.c
                           ${SDL2_SRC_DIR}/render/SDL_d3dmath.c
                           ${SDL2_SRC_DIR}/render/SDL_render.c
                           ${SDL2_SRC_DIR}/render/SDL_yuv_mmx.c
                           ${SDL2_SRC_DIR}/render/SDL_yuv_sw.c
                           ${SDL2_SRC_DIR}/render/software/SDL_blendfillrect.c
                           ${SDL2_SRC_DIR}/render/software/SDL_blendline.c
                           ${SDL2_SRC_DIR}/render/software/SDL_blendpoint.c
                           ${SDL2_SRC_DIR}/render/software/SDL_drawline.c
                           ${SDL2_SRC_DIR}/render/software/SDL_drawpoint.c
                           ${SDL2_SRC_DIR}/render/software/SDL_render_sw.c
                           ${SDL2_SRC_DIR}/render/software/SDL_rotate.c
                           ${SDL2_SRC_DIR}/SDL.c
                           ${SDL2_SRC_DIR}/SDL_assert.c
                           ${SDL2_SRC_DIR}/SDL_error.c
                           ${SDL2_SRC_DIR}/SDL_hints.c
                           ${SDL2_SRC_DIR}/SDL_log.c
                           ${SDL2_SRC_DIR}/stdlib/SDL_getenv.c
                           ${SDL2_SRC_DIR}/stdlib/SDL_iconv.c
                           ${SDL2_SRC_DIR}/stdlib/SDL_malloc.c
                           ${SDL2_SRC_DIR}/stdlib/SDL_qsort.c
                           ${SDL2_SRC_DIR}/stdlib/SDL_stdlib.c
                           ${SDL2_SRC_DIR}/stdlib/SDL_string.c
                           ${SDL2_SRC_DIR}/thread/generic/SDL_syscond.c
                           ${SDL2_SRC_DIR}/thread/generic/SDL_sysmutex.c
                           ${SDL2_SRC_DIR}/thread/generic/SDL_syssem.c
                           ${SDL2_SRC_DIR}/thread/generic/SDL_systhread.c
                           ${SDL2_SRC_DIR}/thread/generic/SDL_systls.c
                           ${SDL2_SRC_DIR}/thread/SDL_thread.c
                           ${SDL2_SRC_DIR}/timer/SDL_timer.c
                           ${SDL2_SRC_DIR}/timer/windows/SDL_systimer.c
                           ${SDL2_SRC_DIR}/video/SDL_blit.c
                           ${SDL2_SRC_DIR}/video/SDL_blit_0.c
                           ${SDL2_SRC_DIR}/video/SDL_blit_1.c
                           ${SDL2_SRC_DIR}/video/SDL_blit_A.c
                           ${SDL2_SRC_DIR}/video/SDL_blit_auto.c
                           ${SDL2_SRC_DIR}/video/SDL_blit_copy.c
                           ${SDL2_SRC_DIR}/video/SDL_blit_N.c
                           ${SDL2_SRC_DIR}/video/SDL_blit_slow.c
                           ${SDL2_SRC_DIR}/video/SDL_bmp.c
                           ${SDL2_SRC_DIR}/video/SDL_clipboard.c
                           ${SDL2_SRC_DIR}/video/SDL_egl.c
                           ${SDL2_SRC_DIR}/video/SDL_fillrect.c
                           ${SDL2_SRC_DIR}/video/SDL_pixels.c
                           ${SDL2_SRC_DIR}/video/SDL_rect.c
                           ${SDL2_SRC_DIR}/video/SDL_RLEaccel.c
                           ${SDL2_SRC_DIR}/video/SDL_shape.c
                           ${SDL2_SRC_DIR}/video/SDL_stretch.c
                           ${SDL2_SRC_DIR}/video/SDL_surface.c
                           ${SDL2_SRC_DIR}/video/SDL_video.c
                           ${SDL2_SRC_DIR}/video/windows/SDL_windowsclipboard.c
                           ${SDL2_SRC_DIR}/video/windows/SDL_windowsevents.c
                           ${SDL2_SRC_DIR}/video/windows/SDL_windowsframebuffer.c
                           ${SDL2_SRC_DIR}/video/windows/SDL_windowskeyboard.c
                           ${SDL2_SRC_DIR}/video/windows/SDL_windowsmessagebox.c
                           ${SDL2_SRC_DIR}/video/windows/SDL_windowsmodes.c
                           ${SDL2_SRC_DIR}/video/windows/SDL_windowsmouse.c
                           ${SDL2_SRC_DIR}/video/windows/SDL_windowsopengl.c
                           ${SDL2_SRC_DIR}/video/windows/SDL_windowsopengles.c
                           ${SDL2_SRC_DIR}/video/windows/SDL_windowsshape.c
                           ${SDL2_SRC_DIR}/video/windows/SDL_windowsvideo.c
                           ${SDL2_SRC_DIR}/video/windows/SDL_windowswindow.c)
        add_library(SDL2 SHARED ${SDL2_SRC_FILES})
        set(SDL2_INCLUDE_DIR "SDL2/include" ${SDL2_INCLUDE_DIR})
        target_include_directories(SDL2 SYSTEM PRIVATE ${SDL2_INCLUDE_DIR})
        target_link_libraries(SDL2 winmm imm32 version)
        if(MSVC)
            if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 19.0) # Visual Studio 2015 and above
                target_link_libraries(SDL2 winmm imm32 version vcruntime ucrtd) # libucrt
            endif()
        endif()
        # SDL2main
        add_library(SDL2main STATIC ${SDL2_SRC_DIR}/main/windows/SDL_windows_main.c)
        target_include_directories(SDL2main SYSTEM PRIVATE ${SDL2_INCLUDE_DIR})

        set(SDL2_LIBRARY SDL2 SDL2main)
    endif()
else()
    find_package(SDL2 REQUIRED)
endif()
if(SDL2IMAGEFROMSOURCE)
    message("SDL2_image will making from source")
    if(CMAKE_HOST_WIN32)
        set(SDL2_IMAGE_SRC_FILES ${SDL2IMAGE_INCLUDE_DIR}/IMG.c
                                 ${SDL2IMAGE_INCLUDE_DIR}/IMG_bmp.c
                                 ${SDL2IMAGE_INCLUDE_DIR}/IMG_gif.c
                                 ${SDL2IMAGE_INCLUDE_DIR}/IMG_jpg.c
                                 ${SDL2IMAGE_INCLUDE_DIR}/IMG_lbm.c
                                 ${SDL2IMAGE_INCLUDE_DIR}/IMG_pcx.c
                                 ${SDL2IMAGE_INCLUDE_DIR}/IMG_png.c
                                 ${SDL2IMAGE_INCLUDE_DIR}/IMG_pnm.c
                                 ${SDL2IMAGE_INCLUDE_DIR}/IMG_tga.c
                                 ${SDL2IMAGE_INCLUDE_DIR}/IMG_tif.c
                                 ${SDL2IMAGE_INCLUDE_DIR}/IMG_webp.c
                                 ${SDL2IMAGE_INCLUDE_DIR}/IMG_xcf.c
                                 ${SDL2IMAGE_INCLUDE_DIR}/IMG_xpm.c
                                 ${SDL2IMAGE_INCLUDE_DIR}/IMG_xv.c
                                 ${SDL2IMAGE_INCLUDE_DIR}/IMG_xxx.c)
        add_library(SDL2_image SHARED ${SDL2_IMAGE_SRC_FILES})

        target_compile_definitions(SDL2_image PRIVATE LOAD_BMP)
        target_compile_definitions(SDL2_image PRIVATE LOAD_GIF)
        target_compile_definitions(SDL2_image PRIVATE LOAD_JPG)
        target_compile_definitions(SDL2_image PRIVATE LOAD_JPG_DYNAMIC="libjpeg-9.dll")
        target_compile_definitions(SDL2_image PRIVATE LOAD_LBM)
        target_compile_definitions(SDL2_image PRIVATE LOAD_PCX)
        target_compile_definitions(SDL2_image PRIVATE LOAD_PNG)
        target_compile_definitions(SDL2_image PRIVATE LOAD_PNG_DYNAMIC="libpng16-16.dll")
        target_compile_definitions(SDL2_image PRIVATE LOAD_PNM)
        target_compile_definitions(SDL2_image PRIVATE LOAD_TGA)
        target_compile_definitions(SDL2_image PRIVATE LOAD_TIF)
        target_compile_definitions(SDL2_image PRIVATE LOAD_TIF_DYNAMIC="libtiff-5.dll")
        target_compile_definitions(SDL2_image PRIVATE LOAD_WEBP)
        target_compile_definitions(SDL2_image PRIVATE LOAD_WEBP_DYNAMIC="libwebp-4.dll")
        target_compile_definitions(SDL2_image PRIVATE LOAD_XPM)
        target_compile_definitions(SDL2_image PRIVATE LOAD_XV)
        target_compile_definitions(SDL2_image PRIVATE PNG_USE_DLL)
        target_compile_definitions(SDL2_image PRIVATE ZLIB_DLL)

        target_include_directories(SDL2_image SYSTEM PRIVATE ${SDL2_INCLUDE_DIR}
                                                             ${SDL2IMAGE_INCLUDE_DIR}
                                                             ${SDL2IMAGE_INCLUDE_DIR}/VisualC/external/include
                                                             ${SDL2IMAGE_INCLUDE_DIR}/external/tiff-4.0.3/libtiff
                                                             ${SDL2IMAGE_INCLUDE_DIR}/external/libwebp-0.3.0/src
                                                             ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2
                                                             ${SDL2IMAGE_INCLUDE_DIR}/external/jpeg-9)

        target_link_libraries(SDL2_image ${SDL2_LIBRARY})
        set(SDL2IMAGE_LIBRARY SDL2_image)
        if(CMAKE_CL_64)
            set(SDL2IMAGE_LIBPNG_DLL ${SDL2IMAGE_INCLUDE_DIR}/VisualC/external/lib/x64/libpng16-16.dll)
            set(SDL2IMAGE_ZLIB_DLL ${SDL2IMAGE_INCLUDE_DIR}/VisualC/external/lib/x64/zlib1.dll)
        else()
            set(SDL2IMAGE_LIBPNG_DLL ${SDL2IMAGE_INCLUDE_DIR}/VisualC/external/lib/x86/libpng16-16.dll)
            set(SDL2IMAGE_ZLIB_DLL ${SDL2IMAGE_INCLUDE_DIR}/VisualC/external/lib/x86/zlib1.dll)
        endif()
    endif()
else()
    find_package(SDL2IMAGE REQUIRED)
endif()
if(FREETYPEFROMSOURCE)
    message("freetype will making from source")
    if(CMAKE_HOST_WIN32)
        set(FREETYPE_SRC_FILES ${FREETYPE_SRC_DIR}/../builds/windows/ftdebug.c
                               ${FREETYPE_SRC_DIR}/autofit/autofit.c
                               ${FREETYPE_SRC_DIR}/base/ftbase.c
                               ${FREETYPE_SRC_DIR}/base/ftbbox.c
                               ${FREETYPE_SRC_DIR}/base/ftbitmap.c
                               ${FREETYPE_SRC_DIR}/base/ftfntfmt.c
                               ${FREETYPE_SRC_DIR}/base/ftfstype.c
                               ${FREETYPE_SRC_DIR}/base/ftgasp.c
                               ${FREETYPE_SRC_DIR}/base/ftglyph.c
                               ${FREETYPE_SRC_DIR}/base/ftgxval.c
                               ${FREETYPE_SRC_DIR}/base/ftinit.c
                               ${FREETYPE_SRC_DIR}/base/ftlcdfil.c
                               ${FREETYPE_SRC_DIR}/base/ftmm.c
                               ${FREETYPE_SRC_DIR}/base/ftotval.c
                               ${FREETYPE_SRC_DIR}/base/ftpatent.c
                               ${FREETYPE_SRC_DIR}/base/ftpfr.c
                               ${FREETYPE_SRC_DIR}/base/ftstroke.c
                               ${FREETYPE_SRC_DIR}/base/ftsynth.c
                               ${FREETYPE_SRC_DIR}/base/ftsystem.c
                               ${FREETYPE_SRC_DIR}/base/fttype1.c
                               ${FREETYPE_SRC_DIR}/base/ftwinfnt.c
                               ${FREETYPE_SRC_DIR}/bdf/bdf.c
                               ${FREETYPE_SRC_DIR}/cache/ftcache.c
                               ${FREETYPE_SRC_DIR}/cff/cff.c
                               ${FREETYPE_SRC_DIR}/cid/type1cid.c
                               ${FREETYPE_SRC_DIR}/gzip/ftgzip.c
                               ${FREETYPE_SRC_DIR}/lzw/ftlzw.c
                               ${FREETYPE_SRC_DIR}/pcf/pcf.c
                               ${FREETYPE_SRC_DIR}/pfr/pfr.c
                               ${FREETYPE_SRC_DIR}/psaux/psaux.c
                               ${FREETYPE_SRC_DIR}/pshinter/pshinter.c
                               ${FREETYPE_SRC_DIR}/psnames/psmodule.c
                               ${FREETYPE_SRC_DIR}/raster/raster.c
                               ${FREETYPE_SRC_DIR}/sfnt/sfnt.c
                               ${FREETYPE_SRC_DIR}/smooth/smooth.c
                               ${FREETYPE_SRC_DIR}/truetype/truetype.c
                               ${FREETYPE_SRC_DIR}/type1/type1.c
                               ${FREETYPE_SRC_DIR}/type42/type42.c
                               ${FREETYPE_SRC_DIR}/winfonts/winfnt.c)
        add_library(freetype SHARED ${FREETYPE_SRC_FILES})

        set(FREETYPE_INCLUDE_DIRS ${FREETYPEDIR}/include ${FREETYPEDIR}/include/freetype2)
        target_include_directories(freetype SYSTEM PRIVATE ${FREETYPE_INCLUDE_DIRS})

        target_compile_definitions(freetype PRIVATE FT2_BUILD_LIBRARY)
        target_compile_definitions(freetype PRIVATE FT_CONFIG_OPTIONS_H="${CMAKE_SOURCE_DIR}/freetype/config/ftoption.h")

        set(FREETYPE_LIBRARIES freetype)
    endif()
else()
    if(NOT UNIX)
        set(FREETYPE_INCLUDE_DIRS ${FREETYPEDIR}/include ${FREETYPEDIR}/include/freetype2)
        if(MSVC)
            set(FREETYPE_LIBRARY ${FREETYPEDIR}/lib/freetype.lib)
        else()
            set(FREETYPE_LIBRARY ${FREETYPEDIR}/lib/libfreetype.dll.a)
        endif()
    endif()
    find_package(Freetype REQUIRED)
    set(FREETYPE_DLL "${FREETYPEDIR}/bin/freetype6.dll")
endif()
# ===========================================

# Compiler setup ============================
if (MSVC)
    set(COMPILER_FLAGS "/W4 /GS")
    set(LINK_FLAGS "${LINK_FLAGS} /SafeSEH /DynamicBase /NXCompat")

    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(-DNOMINMAX)
else()
    set(COMPILER_FLAGS
        "-std=gnu++0x -Wall -Wno-comment -Wno-unknown-pragmas -Wno-unused-parameter -Wextra")
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
file(GLOB LANTERN_HEADERS "lantern/include/*.h")

file(GLOB LANTERN_SOURCES "lantern/src/*.cpp")

set(LANTERN_INCLUDE_FOLDERS
    lantern/include)

add_library(
    lantern STATIC
    ${LANTERN_SOURCES}
    ${LANTERN_HEADERS})

target_include_directories(lantern PUBLIC ${LANTERN_INCLUDE_FOLDERS})
target_include_directories(lantern PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})

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
    # tests/src/pipeline.cpp
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

# Empty app target ==========================
add_executable(
    empty_app WIN32
    examples/empty_app/main.cpp)

set_target_properties(
    empty_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/empty_app")

target_include_directories(empty_app PRIVATE lantern/include)
target_include_directories(empty_app PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})

if(SDL2FROMSOURCE)
    if(MINGW)
        target_compile_definitions(empty_app PRIVATE SDL_MAIN_HANDLED)
    endif()
endif()

target_link_libraries(empty_app lantern ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${FREETYPE_LIBRARIES})

if (WIN32)
    if(SDL2FROMSOURCE)
        add_custom_command(
          TARGET empty_app
          POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
          $<TARGET_FILE:SDL2>
          $<TARGET_FILE_DIR:empty_app>)
    else()
    add_custom_command(
        TARGET empty_app
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL2_DLL}"
        $<TARGET_FILE_DIR:empty_app>)
    endif()

    if(SDL2IMAGEFROMSOURCE)
        add_custom_command(
          TARGET empty_app
          POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
          $<TARGET_FILE:SDL2_image>
          $<TARGET_FILE_DIR:empty_app>)
    else()
    add_custom_command(
        TARGET empty_app
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL2IMAGE_DLL}"
        $<TARGET_FILE_DIR:empty_app>)
    endif()

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

    if(FREETYPEFROMSOURCE)
        add_custom_command(
          TARGET empty_app
          POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
          $<TARGET_FILE:freetype>
          $<TARGET_FILE_DIR:empty_app>)
    else()
    add_custom_command(
        TARGET empty_app
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        "${FREETYPE_DLL}"
        $<TARGET_FILE_DIR:empty_app>)
    endif()
endif()
# ===========================================

# Rasterized triangle app target ============
add_executable(
    rasterized_triangle_app WIN32
    examples/rasterized_triangle_app/main.cpp)

set_target_properties(
    rasterized_triangle_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app")

target_include_directories(rasterized_triangle_app PRIVATE lantern/include)
target_include_directories(rasterized_triangle_app PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})

if(SDL2FROMSOURCE)
    if(MINGW)
        target_compile_definitions(rasterized_triangle_app PRIVATE SDL_MAIN_HANDLED)
    endif()
endif()

target_link_libraries(rasterized_triangle_app lantern ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${FREETYPE_LIBRARIES})

if (WIN32)
    if(SDL2FROMSOURCE)
        add_custom_command(
          TARGET rasterized_triangle_app
          POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
          $<TARGET_FILE:SDL2>
          $<TARGET_FILE_DIR:rasterized_triangle_app>)
    else()
    add_custom_command(
        TARGET rasterized_triangle_app
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL2_DLL}"
        $<TARGET_FILE_DIR:rasterized_triangle_app>)
    endif()

    if(SDL2IMAGEFROMSOURCE)
        add_custom_command(
          TARGET rasterized_triangle_app
          POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
          $<TARGET_FILE:SDL2_image>
          $<TARGET_FILE_DIR:rasterized_triangle_app>)
    else()
    add_custom_command(
        TARGET rasterized_triangle_app
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL2IMAGE_DLL}"
        $<TARGET_FILE_DIR:rasterized_triangle_app>)
    endif()

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

    if(FREETYPEFROMSOURCE)
        add_custom_command(
          TARGET rasterized_triangle_app
          POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
          $<TARGET_FILE:freetype>
          $<TARGET_FILE_DIR:rasterized_triangle_app>)
    else()
    add_custom_command(
        TARGET rasterized_triangle_app
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        "${FREETYPE_DLL}"
        $<TARGET_FILE_DIR:rasterized_triangle_app>)
    endif()
endif()

add_custom_command(
    TARGET rasterized_triangle_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/examples/rasterized_triangle_app/resources"
    $<TARGET_FILE_DIR:rasterized_triangle_app>/resources)
# ===========================================