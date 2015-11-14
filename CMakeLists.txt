cmake_minimum_required(VERSION 2.8.12.2)

# Project name
project(lantern)

# Add testing tool
include(CTest)

# SDL2, SDL2_image and freetype look up =====
set(SDL2FROMSOURCE FALSE)
set(SDL2IMAGEFROMSOURCE FALSE)
set(FREETYPEFROMSOURCE FALSE)

if(DEFINED ENV{SDL2DIR})
    string(REPLACE "\\" "/" SDL2DIR $ENV{SDL2DIR})
elseif(DEFINED SDL2DIR)
    string(REPLACE "\\" "/" SDL2DIR ${SDL2DIR})
endif()

if(WIN32 OR ANDROID)
    if(DEFINED SDL2DIR)
        set(SDL2_INCLUDE_DIR ${SDL2DIR}/include)
        set(SDL2_SRC_DIR ${SDL2DIR}/src)
        if(EXISTS ${SDL2_SRC_DIR}/SDL.c)
            set(SDL2FROMSOURCE TRUE)
        endif()
    endif()
endif()

if(DEFINED ENV{SDL2IMAGEDIR})
    string(REPLACE "\\" "/" SDL2IMAGEDIR $ENV{SDL2IMAGEDIR})
elseif(DEFINED SDL2IMAGEDIR)
    string(REPLACE "\\" "/" SDL2IMAGEDIR ${SDL2IMAGEDIR})
endif()

if(WIN32 OR ANDROID)
    if(DEFINED SDL2IMAGEDIR)
        set(SDL2IMAGE_INCLUDE_DIR ${SDL2IMAGEDIR})
        if(EXISTS ${SDL2IMAGE_INCLUDE_DIR}/IMG.c)
            set(SDL2IMAGEFROMSOURCE TRUE)
        endif()
    endif()
endif()

if(DEFINED ENV{FREETYPEDIR})
    string(REPLACE "\\" "/" FREETYPEDIR $ENV{FREETYPEDIR})
elseif(DEFINED FREETYPEDIR)
    string(REPLACE "\\" "/" FREETYPEDIR ${FREETYPEDIR})
endif()

if(WIN32 OR ANDROID)
    if(DEFINED FREETYPEDIR)
        if(WIN32)
            set(FREETYPE_INCLUDE_DIRS ${FREETYPEDIR}/include ${FREETYPEDIR}/include/freetype2)
        elseif(ANDROID)
           set(FREETYPE_INCLUDE_DIRS ${FREETYPEDIR}/include)
        endif()
        set(FREETYPE_SRC_DIR ${FREETYPEDIR}/src)
        if(EXISTS ${FREETYPE_SRC_DIR}/base/ftapi.c)
            set(FREETYPEFROMSOURCE TRUE)
        endif()
    endif()
elseif(UNIX)
    set(FREETYPE_INCLUDE_DIRS /usr/include/freetype2)
endif()

if(ANDROID)
    if((NOT SDL2FROMSOURCE) OR (NOT SDL2IMAGEFROMSOURCE) OR (NOT FREETYPEFROMSOURCE))
        message(FATAL_ERROR "Android build required sources of SDL2, SDL2 Image and freetype libraries")
    endif()
    include(${CMAKE_SOURCE_DIR}/cmake/AndroidMacros.cmake)
else()
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
endif()

if(SDL2FROMSOURCE)
    message("SDL2 will making from source")
    # SDL2
    set(SDL2_SRC_FILES ${SDL2_SRC_DIR}/atomic/SDL_atomic.c
                       ${SDL2_SRC_DIR}/atomic/SDL_spinlock.c
                       ${SDL2_SRC_DIR}/audio/SDL_audio.c
                       ${SDL2_SRC_DIR}/audio/SDL_audiocvt.c
                       ${SDL2_SRC_DIR}/audio/SDL_audiodev.c
                       ${SDL2_SRC_DIR}/audio/SDL_audiotypecvt.c
                       ${SDL2_SRC_DIR}/audio/SDL_mixer.c
                       ${SDL2_SRC_DIR}/audio/SDL_wave.c
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
                       ${SDL2_SRC_DIR}/file/SDL_rwops.c
                       ${SDL2_SRC_DIR}/haptic/SDL_haptic.c
                       ${SDL2_SRC_DIR}/joystick/SDL_gamecontroller.c
                       ${SDL2_SRC_DIR}/joystick/SDL_joystick.c
                       ${SDL2_SRC_DIR}/power/SDL_power.c
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
                       ${SDL2_SRC_DIR}/thread/SDL_thread.c
                       ${SDL2_SRC_DIR}/timer/SDL_timer.c
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
                       ${SDL2_SRC_DIR}/video/SDL_video.c)
    if(WIN32)
        list(APPEND SDL2_SRC_FILES ${SDL2_SRC_DIR}/core/windows/SDL_windows.c
                                   ${SDL2_SRC_DIR}/filesystem/windows/SDL_sysfilesystem.c
                                   ${SDL2_SRC_DIR}/haptic/windows/SDL_syshaptic.c
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
                                   ${SDL2_SRC_DIR}/power/windows/SDL_syspower.c
                                   ${SDL2_SRC_DIR}/render/direct3d/SDL_render_d3d.c
                                   ${SDL2_SRC_DIR}/render/opengl/SDL_render_gl.c
                                   ${SDL2_SRC_DIR}/render/opengl/SDL_shaders_gl.c
                                   ${SDL2_SRC_DIR}/render/SDL_d3dmath.c
                                   ${SDL2_SRC_DIR}/thread/generic/SDL_syscond.c
                                   ${SDL2_SRC_DIR}/thread/generic/SDL_sysmutex.c
                                   ${SDL2_SRC_DIR}/thread/generic/SDL_syssem.c
                                   ${SDL2_SRC_DIR}/thread/generic/SDL_systhread.c
                                   ${SDL2_SRC_DIR}/thread/generic/SDL_systls.c
                                   ${SDL2_SRC_DIR}/timer/windows/SDL_systimer.c
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

        target_link_libraries(SDL2 winmm imm32 version)
        if(MSVC)
            if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 19.0) # Visual Studio 2015 and above
                target_link_libraries(SDL2 winmm imm32 version vcruntime ucrtd) # libucrt
            endif()
        endif()

    elseif(ANDROID)
        list(APPEND SDL2_SRC_FILES ${SDL2_SRC_DIR}/audio/android/SDL_androidaudio.c
                                   ${SDL2_SRC_DIR}/audio/dummy/SDL_dummyaudio.c
                                   ${SDL2_SRC_DIR}/core/android/SDL_android.c
                                   ${SDL2_SRC_DIR}/filesystem/dummy/SDL_sysfilesystem.c
                                   ${SDL2_SRC_DIR}/haptic/dummy/SDL_syshaptic.c
                                   ${SDL2_SRC_DIR}/joystick/android/SDL_sysjoystick.c
                                   ${SDL2_SRC_DIR}/loadso/dlopen/SDL_sysloadso.c
                                   ${SDL2_SRC_DIR}/power/android/SDL_syspower.c
                                   ${SDL2_SRC_DIR}/render/opengles/SDL_render_gles.c
                                   ${SDL2_SRC_DIR}/render/opengles2/SDL_render_gles2.c
                                   ${SDL2_SRC_DIR}/render/opengles2/SDL_shaders_gles2.c
                                   ${SDL2_SRC_DIR}/thread/pthread/SDL_syscond.c
                                   ${SDL2_SRC_DIR}/thread/pthread/SDL_sysmutex.c
                                   ${SDL2_SRC_DIR}/thread/pthread/SDL_syssem.c
                                   ${SDL2_SRC_DIR}/thread/pthread/SDL_systhread.c
                                   ${SDL2_SRC_DIR}/thread/pthread/SDL_systls.c
                                   ${SDL2_SRC_DIR}/timer/unix/SDL_systimer.c
                                   ${SDL2_SRC_DIR}/video/android/SDL_androidclipboard.c
                                   ${SDL2_SRC_DIR}/video/android/SDL_androidevents.c
                                   ${SDL2_SRC_DIR}/video/android/SDL_androidgl.c
                                   ${SDL2_SRC_DIR}/video/android/SDL_androidkeyboard.c
                                   ${SDL2_SRC_DIR}/video/android/SDL_androidtouch.c
                                   ${SDL2_SRC_DIR}/video/android/SDL_androidvideo.c
                                   ${SDL2_SRC_DIR}/video/android/SDL_androidwindow.c)

          add_library(SDL2 STATIC ${SDL2_SRC_FILES})
          target_compile_definitions(SDL2 PRIVATE GL_GLEXT_PROTOTYPES)
    endif()

    target_include_directories(SDL2 SYSTEM PRIVATE ${SDL2_INCLUDE_DIR})

    # SDL2main
    if(WIN32)
        add_library(SDL2main STATIC ${SDL2_SRC_DIR}/main/windows/SDL_windows_main.c)
        target_include_directories(SDL2main SYSTEM PRIVATE ${SDL2_INCLUDE_DIR})
        set(SDL2_LIBRARY SDL2 SDL2main)
    elseif(ANDROID)
        # add_library(SDL2main STATIC ${SDL2_SRC_DIR}/main/android/SDL_android_main.c)
        set(SDL2_LIBRARY SDL2)
    endif()
else()
    find_package(SDL2 REQUIRED)
endif()

if(SDL2IMAGEFROMSOURCE)
    message("SDL2_image will making from source")
    # SDL2_image
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
    if(WIN32)
        add_library(SDL2_image SHARED ${SDL2_IMAGE_SRC_FILES})

        target_compile_definitions(SDL2_image PRIVATE LOAD_JPG)
        target_compile_definitions(SDL2_image PRIVATE LOAD_JPG_DYNAMIC="libjpeg-9.dll")
        target_compile_definitions(SDL2_image PRIVATE LOAD_PNG)
        target_compile_definitions(SDL2_image PRIVATE LOAD_PNG_DYNAMIC="libpng16-16.dll")
        target_compile_definitions(SDL2_image PRIVATE LOAD_TIF)
        target_compile_definitions(SDL2_image PRIVATE LOAD_TIF_DYNAMIC="libtiff-5.dll")
        target_compile_definitions(SDL2_image PRIVATE LOAD_WEBP)
        target_compile_definitions(SDL2_image PRIVATE LOAD_WEBP_DYNAMIC="libwebp-4.dll")
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

        if(CMAKE_CL_64)
            set(SDL2IMAGE_LIBPNG_DLL ${SDL2IMAGE_INCLUDE_DIR}/VisualC/external/lib/x64/libpng16-16.dll)
            set(SDL2IMAGE_ZLIB_DLL ${SDL2IMAGE_INCLUDE_DIR}/VisualC/external/lib/x64/zlib1.dll)
        else()
            set(SDL2IMAGE_LIBPNG_DLL ${SDL2IMAGE_INCLUDE_DIR}/VisualC/external/lib/x86/libpng16-16.dll)
            set(SDL2IMAGE_ZLIB_DLL ${SDL2IMAGE_INCLUDE_DIR}/VisualC/external/lib/x86/zlib1.dll)
        endif()
    elseif(ANDROID)
      list(APPEND SDL2_IMAGE_SRC_FILES ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/png.c
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
                                       ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2/pngwutil.c)

        add_library(SDL2_image STATIC ${SDL2_IMAGE_SRC_FILES})

        target_compile_definitions(SDL2_image PRIVATE LOAD_PNG)
        target_compile_definitions(SDL2_image PRIVATE LOAD_XCF)

        target_include_directories(SDL2_image SYSTEM PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR}
                                                             ${SDL2IMAGE_INCLUDE_DIR}/external/libpng-1.6.2)
    endif()

    target_compile_definitions(SDL2_image PRIVATE LOAD_BMP)
    target_compile_definitions(SDL2_image PRIVATE LOAD_GIF)
    target_compile_definitions(SDL2_image PRIVATE LOAD_LBM)
    target_compile_definitions(SDL2_image PRIVATE LOAD_PCX)
    target_compile_definitions(SDL2_image PRIVATE LOAD_PNM)
    target_compile_definitions(SDL2_image PRIVATE LOAD_TGA)
    target_compile_definitions(SDL2_image PRIVATE LOAD_XPM)
    target_compile_definitions(SDL2_image PRIVATE LOAD_XV)

    set(SDL2IMAGE_LIBRARY SDL2_image)
else()
    find_package(SDL2IMAGE REQUIRED)
endif()

if(FREETYPEFROMSOURCE)
    message("freetype will making from source")
    # freetype
    set(FREETYPE_SRC_FILES ${FREETYPE_SRC_DIR}/autofit/autofit.c
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
                           ${FREETYPE_SRC_DIR}/raster/raster.c
                           ${FREETYPE_SRC_DIR}/sfnt/sfnt.c
                           ${FREETYPE_SRC_DIR}/smooth/smooth.c
                           ${FREETYPE_SRC_DIR}/truetype/truetype.c
                           ${FREETYPE_SRC_DIR}/type1/type1.c
                           ${FREETYPE_SRC_DIR}/type42/type42.c
                           ${FREETYPE_SRC_DIR}/winfonts/winfnt.c)
    if(WIN32)
        list(APPEND FREETYPE_SRC_FILES ${FREETYPE_SRC_DIR}/../builds/windows/ftdebug.c
                                       ${FREETYPE_SRC_DIR}/base/ftbase.c
                                       ${FREETYPE_SRC_DIR}/psnames/psmodule.c)

        add_library(freetype SHARED ${FREETYPE_SRC_FILES})
        target_compile_definitions(freetype PRIVATE FT_CONFIG_OPTIONS_H="${CMAKE_SOURCE_DIR}/freetype/config/ftoption.h")
    elseif(ANDROID)
        list(APPEND FREETYPE_SRC_FILES ${FREETYPE_SRC_DIR}/base/ftadvanc.c
                                       ${FREETYPE_SRC_DIR}/base/ftbdf.c
                                       ${FREETYPE_SRC_DIR}/base/ftcalc.c
                                       ${FREETYPE_SRC_DIR}/base/ftgloadr.c
                                       ${FREETYPE_SRC_DIR}/base/ftobjs.c
                                       ${FREETYPE_SRC_DIR}/base/ftoutln.c
                                       ${FREETYPE_SRC_DIR}/base/ftrfork.c
                                       ${FREETYPE_SRC_DIR}/base/ftstream.c
                                       ${FREETYPE_SRC_DIR}/base/fttrigon.c
                                       ${FREETYPE_SRC_DIR}/base/ftutil.c
                                       ${FREETYPE_SRC_DIR}/bzip2/ftbzip2.c
                                       ${FREETYPE_SRC_DIR}/gzip/zutil.c
                                       ${FREETYPE_SRC_DIR}/psnames/psnames.c
                                   ${SDL2IMAGE_INCLUDE_DIR}/external/zlib-1.2.8/zutil.c)

        add_library(freetype STATIC ${FREETYPE_SRC_FILES})
    endif()

    target_compile_definitions(freetype PRIVATE FT2_BUILD_LIBRARY)
    target_include_directories(freetype SYSTEM PRIVATE ${FREETYPE_INCLUDE_DIRS})

    set(FREETYPE_LIBRARIES freetype)
else()
    if(WIN32)
        if(MSVC)
            set(FREETYPE_LIBRARY ${FREETYPEDIR}/lib/freetype.lib)
        else()
            set(FREETYPE_LIBRARY ${FREETYPEDIR}/lib/libfreetype.dll.a)
        endif()
    endif()
    find_package(Freetype REQUIRED)
    if(WIN32)
        set(FREETYPE_DLL "${FREETYPEDIR}/bin/freetype6.dll")
    endif()
endif()
# ===========================================

# Compiler setup ============================
if (MSVC)
    set(COMPILER_C_FLAGS "/W4 /GS")
    set(COMPILER_CXX_FLAGS "/W4 /GS")

    if(CMAKE_CL_64)
      set(LINK_FLAGS "${LINK_FLAGS} /DynamicBase /NXCompat")
    else()
      set(LINK_FLAGS "${LINK_FLAGS} /SafeSEH /DynamicBase /NXCompat")
    endif()

    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(-DNOMINMAX)
else()
    set(COMPILER_C_FLAGS
        "-std=c11 -Wall -Wno-comment -Wno-unknown-pragmas -Wno-unused-parameter -Wextra")
    set(COMPILER_CXX_FLAGS
        "-std=gnu++0x -Wall -Wno-comment -Wno-unknown-pragmas -Wno-unused-parameter -Wextra")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEFAULT_CMAKE_C_FLAGS} ${COMPILER_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEFAULT_CMAKE_CXX_FLAGS} ${COMPILER_CXX_FLAGS}")

if(NOT ANDROID)
    if(CMAKE_HOST_WIN32)
        add_definitions(-DWIN32)
        add_definitions(-D_WIN32)
        add_definitions(-D_WINDOWS)
    endif()

    if(NOT MSVC)
        add_definitions(-DGTEST_HAS_PTHREAD=0)
        add_definitions(-U__STRICT_ANSI__)
    endif()
endif()
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

if(NOT ANDROID)
    set_target_properties(
        lantern PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif()
# ===========================================

# Gooogle C++ testing framework target ======
if(ANDROID)
    set(GTEST_ROOT ${NDK_PATH}/sources/third_party/googletest/googletest)
else()
    if(DEFINED ENV{GTEST_ROOT})
        string(REPLACE "\\" "/" GTEST_ROOT $ENV{GTEST_ROOT})
    elseif(DEFINED GTEST_ROOT)
        string(REPLACE "\\" "/" GTEST_ROOT ${GTEST_ROOT})
    endif()
endif()

if(DEFINED GTEST_ROOT)

    set(GTEST_LIBRARY ${GTEST_ROOT})
    set(GTEST_INCLUDE_DIR ${GTEST_ROOT}/include)
    set(GTEST_MAIN_LIBRARY gtest)
    find_package(GTest REQUIRED)

    set(GTEST_SOURCES ${GTEST_ROOT}/src/gtest-all.cc)

    add_library(
        ${GTEST_MAIN_LIBRARY} STATIC
        ${GTEST_SOURCES})

    target_include_directories(${GTEST_MAIN_LIBRARY} PRIVATE ${GTEST_LIBRARY} ${GTEST_INCLUDE_DIR})

    if(UNIX AND NOT ANDROID)
        target_link_libraries(${GTEST_MAIN_LIBRARY} pthread)
    endif()

endif()
# ===========================================

# Tests target ==============================
if (DEFINED GTEST_ROOT)
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

    target_include_directories(tests PRIVATE lantern/include
                                             tests/include
                                             ${GTEST_INCLUDE_DIR})


    target_link_libraries(tests lantern gtest)

    add_custom_command(
        TARGET tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/tests/resources"
        $<TARGET_FILE_DIR:tests>/resources)

    if(ANDROID)
        add_custom_target(run_tests
            DEPENDS tests
            COMMAND adb push $<TARGET_FILE_DIR:tests> /data/local/tmp
            COMMAND adb shell chmod 775 /data/local/tmp/tests
            COMMAND adb shell "cd /data/local/tmp ; /data/local/tmp/tests")
    endif()
endif()
# ===========================================

# Empty app target ==========================
if(ANDROID)
    file(GLOB target_source_files examples/empty_app/*.cpp
                                  ${SDL2_SRC_DIR}/main/android/SDL_android_main.c)
    list(APPEND target_include_dirs ${LANTERN_INCLUDE_FOLDERS}
                                    ${SDL2_INCLUDE_DIR}
                                    ${SDL2_SRC_DIR}
                                    ${SDL2_IMAGE_INCLUDE_DIR}
                                    ${FREETYPE_INCLUDE_DIRS})
    list(APPEND target_link_libs lantern
                                 ${SDL2IMAGE_LIBRARY}
                                 ${SDL2_LIBRARY}
                                 ${FREETYPE_LIBRARIES}
                                 GLESv1_CM
                                 GLESv2
                                 log
                                 android
                                 z)

    add_Android_Project(empty_app
                        EmptyAppNativeActivity
                        org.lantern
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
endif()
# ===========================================

# Rasterized triangle app target ============
if(ANDROID)
    file(GLOB target_source_files examples/rasterized_triangle_app/*.cpp
                                  ${SDL2_SRC_DIR}/main/android/SDL_android_main.c)
    list(APPEND target_include_dirs ${LANTERN_INCLUDE_FOLDERS}
                                    ${SDL2_INCLUDE_DIR}
                                    ${SDL2_SRC_DIR}
                                    ${SDL2_IMAGE_INCLUDE_DIR}
                                    ${FREETYPE_INCLUDE_DIRS})
    list(APPEND target_link_libs lantern
                                 ${SDL2IMAGE_LIBRARY}
                                 ${SDL2_LIBRARY}
                                 ${FREETYPE_LIBRARIES}
                                 GLESv1_CM
                                 GLESv2
                                 log
                                 android
                                 z)

    add_Android_Project(rasterized_triangle_app
                        RasterizedTriangleAppNativeActivity
                        org.lantern
                        ${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app
                        "${target_source_files}"
                        "${target_include_dirs}"
                        "${target_link_libs}")
else()
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
endif()
# ===========================================