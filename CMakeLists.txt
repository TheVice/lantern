cmake_minimum_required(VERSION 2.8.12)

# Project name
project(lantern)

# Add testing tool
include(CTest)

# SDL2 look up ==============================
if(NOT ANDROID)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
    find_package(SDL2 REQUIRED)
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
    set(NDK_PATH $ENV{ANDROID_NDK})
    set(ANDROID_EXECUTABLE android)
    set(ANT_EXECUTABLE ant)

    if(CMAKE_HOST_WIN32)
        string(REPLACE "\\" "/" NDK_PATH ${NDK_PATH})
        set(ANDROID_EXECUTABLE android.bat)
        set(ANT_EXECUTABLE ant.bat)
    endif(CMAKE_HOST_WIN32)

    string(TOLOWER ${CMAKE_BUILD_TYPE} ant_build_type)

    add_library(android_native_app_glue STATIC
                "${NDK_PATH}/sources/android/native_app_glue/android_native_app_glue.c")

    target_include_directories(android_native_app_glue SYSTEM PRIVATE "$ENV{ANDROID_NDK}/sources/android/native_app_glue")
endif(ANDROID)
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
if(NOT ANDROID)
    target_include_directories(lantern PRIVATE ${SDL2_INCLUDE_DIR})
endif(NOT ANDROID)

set_target_properties(
    lantern PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

if(ANDROID)
    target_include_directories(lantern SYSTEM PRIVATE "$ENV{ANDROID_NDK}/sources/android/native_app_glue")
endif(ANDROID)
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
else(ANDROID)
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

if(NOT ANDROID)
    if (DEFINED ENV{GTEST_ROOT})
        target_include_directories(tests PRIVATE $ENV{GTEST_ROOT}/include)
    endif()
endif(NOT ANDROID)

set_target_properties(
    tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

if(NOT ANDROID)
    target_link_libraries(tests lantern gtest)
endif(NOT ANDROID)

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
        COMMAND adb shell "cd /data/local/tmp ; /data/local/tmp/tests"
    )
endif(ANDROID)
# ===========================================

# Empty app target ==========================
if(ANDROID)
    add_library(empty_app SHARED
        examples/empty_app/main.cpp)
    target_include_directories(empty_app SYSTEM PRIVATE "$ENV{ANDROID_NDK}/sources/android/native_app_glue")

    target_link_libraries(empty_app lantern android_native_app_glue android log)

    execute_process(COMMAND ${ANDROID_EXECUTABLE} create project -n empty_app -a LaternEmptyAppNativeActivity -k com.android.lantern.empty_app -t android-${ANDROID_NATIVE_API_LEVEL} -p ${CMAKE_BINARY_DIR}/examples/empty_app)
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/examples/empty_app/res/layout)
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/examples/empty_app/src/com)

    FILE(READ ${CMAKE_BINARY_DIR}/examples/empty_app/AndroidManifest.xml file_content)
    string(REPLACE "android:versionName=\"1.0\">" "android:versionName=\"1.0\">\n    <uses-sdk\n        android:minSdkVersion=\"${ANDROID_NATIVE_API_LEVEL}\" />" file_content ${file_content})
    string(REPLACE "<activity android:name=\"LaternEmptyAppNativeActivity\"" "<activity android:name=\"android.app.NativeActivity\"" file_content ${file_content})
    string(REPLACE "android:label=\"@string/app_name\">" "android:label=\"@string/app_name\">\n            <meta-data android:name=\"android.app.lib_name\"\n                       android:value=\"empty_app\" />" file_content ${file_content})
    string(REPLACE "</manifest> " "</manifest>" file_content ${file_content})
    FILE(WRITE ${CMAKE_BINARY_DIR}/examples/empty_app/AndroidManifest.xml ${file_content})

    FILE(READ ${CMAKE_BINARY_DIR}/examples/empty_app/res/values/strings.xml file_content)
    string(REPLACE "<string name=\"app_name\">LaternEmptyAppNativeActivity</string>" "<string name=\"app_name\">empty_app</string>" file_content ${file_content})
    FILE(WRITE ${CMAKE_BINARY_DIR}/examples/empty_app/res/values/strings.xml ${file_content})

    add_custom_target(empty_app[build-apk]
        DEPENDS empty_app
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_ABI}/libempty_app.so ${CMAKE_BINARY_DIR}/examples/empty_app/libs/${ANDROID_ABI}/libempty_app.so
        COMMAND ${ANT_EXECUTABLE} ${ant_build_type} -buildfile ${CMAKE_BINARY_DIR}/examples/empty_app
    )

    add_custom_target(empty_app[install-apk]
        DEPENDS empty_app[build-apk]
        COMMAND adb install -r ${CMAKE_BINARY_DIR}/examples/empty_app/bin/empty_app-${ant_build_type}.apk
    )

    add_custom_target(empty_app[run-apk]
        DEPENDS empty_app[install-apk]
        COMMAND adb shell am start -a android.intent.action.MAIN -n com.android.lantern.empty_app/android.app.NativeActivity
    )

    add_custom_target(empty_app[uninstall-apk]
        COMMAND adb uninstall com.android.lantern.empty_app
    )
else(ANDROID)
    add_executable(
        empty_app
        examples/empty_app/main.cpp)

    set_target_properties(
        empty_app PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/empty_app")

    target_include_directories(empty_app PRIVATE lantern/include)
    target_include_directories(empty_app PRIVATE ${SDL2_INCLUDE_DIR})

    target_link_libraries(empty_app lantern ${SDL2_LIBRARY})

    if (WIN32)
        add_custom_command(
            TARGET empty_app
            POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
            "${SDL2_DLL}"
            $<TARGET_FILE_DIR:empty_app>)
    endif()
endif(ANDROID)
# ===========================================

# Rasterized triangle app target ======
if(ANDROID)
    add_library(rasterized_triangle_app SHARED
        examples/rasterized_triangle_app/main.cpp)
    target_include_directories(rasterized_triangle_app SYSTEM PRIVATE "$ENV{ANDROID_NDK}/sources/android/native_app_glue")

    target_link_libraries(rasterized_triangle_app lantern android_native_app_glue android log)

    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/examples/rasterized_triangle_app/resources" "${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app/assets/resources")
    execute_process(COMMAND ${ANDROID_EXECUTABLE} create project -n rasterized_triangle_app -a LaternRasterizedTriangleAppNativeActivity -k com.android.lantern.rasterized_triangle_app -t android-${ANDROID_NATIVE_API_LEVEL} -p ${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app)
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app/res/layout)
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app/src/com)

    FILE(READ ${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app/AndroidManifest.xml file_content)
    string(REPLACE "android:versionName=\"1.0\">" "android:versionName=\"1.0\">\n    <uses-sdk\n        android:minSdkVersion=\"${ANDROID_NATIVE_API_LEVEL}\" />" file_content ${file_content})
    string(REPLACE "<activity android:name=\"LaternRasterizedTriangleAppNativeActivity\"" "<activity android:name=\"android.app.NativeActivity\"" file_content ${file_content})
    string(REPLACE "android:label=\"@string/app_name\">" "android:label=\"@string/app_name\">\n            <meta-data android:name=\"android.app.lib_name\"\n                       android:value=\"rasterized_triangle_app\" />" file_content ${file_content})
    string(REPLACE "</manifest> " "</manifest>" file_content ${file_content})
    FILE(WRITE ${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app/AndroidManifest.xml ${file_content})

    FILE(READ ${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app/res/values/strings.xml file_content)
    string(REPLACE "<string name=\"app_name\">LaternRasterizedTriangleAppNativeActivity</string>" "<string name=\"app_name\">rasterized_triangle_app</string>" file_content ${file_content})
    FILE(WRITE ${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app/res/values/strings.xml ${file_content})

    add_custom_target(rasterized_triangle_app[build-apk]
        DEPENDS rasterized_triangle_app
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_ABI}/librasterized_triangle_app.so ${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app/libs/${ANDROID_ABI}/librasterized_triangle_app.so
        COMMAND ${ANT_EXECUTABLE} ${ant_build_type} -buildfile ${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app
    )

    add_custom_target(rasterized_triangle_app[install-apk]
        DEPENDS rasterized_triangle_app[build-apk]
        COMMAND adb install -r ${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app/bin/rasterized_triangle_app-${ant_build_type}.apk
    )

    add_custom_target(rasterized_triangle_app[run-apk]
        DEPENDS rasterized_triangle_app[install-apk]
        COMMAND adb shell am start -a android.intent.action.MAIN -n com.android.lantern.rasterized_triangle_app/android.app.NativeActivity
    )

    add_custom_target(rasterized_triangle_app[uninstall-apk]
        COMMAND adb uninstall com.android.lantern.rasterized_triangle_app
    )
else(ANDROID)
    add_executable(
        rasterized_triangle_app
        examples/rasterized_triangle_app/main.cpp)

    set_target_properties(
        rasterized_triangle_app PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app")

    target_include_directories(rasterized_triangle_app PRIVATE lantern/include)
    target_include_directories(rasterized_triangle_app PRIVATE ${SDL2_INCLUDE_DIR})

    target_link_libraries(rasterized_triangle_app lantern ${SDL2_LIBRARY})

    if (WIN32)
        add_custom_command(
            TARGET rasterized_triangle_app
            POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
            "${SDL2_DLL}"
            $<TARGET_FILE_DIR:rasterized_triangle_app>)
    endif()

    add_custom_command(
        TARGET rasterized_triangle_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/examples/rasterized_triangle_app/resources"
        $<TARGET_FILE_DIR:rasterized_triangle_app>/resources)
endif(ANDROID)
# ===========================================