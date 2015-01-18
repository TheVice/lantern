LOCAL_PATH := $(call my-dir)

#
include $(CLEAR_VARS)
LOCAL_CFLAGS := -std=c++11 -Wall -Wno-comment -Werror -fexceptions
LOCAL_MODULE := runnablelanterntests
LOCAL_C_INCLUDES += ../../../lantern/include/ ../../../tests/include/
LOCAL_SRC_FILES := ../../../../lantern/src/bitmap_painter.cpp \
				   ../../../../lantern/src/camera.cpp \
				   ../../../../lantern/src/color.cpp \
				   ../../../../lantern/src/matrix3x3.cpp \
				   ../../../../lantern/src/matrix4x4.cpp \
				   ../../../../lantern/src/mesh.cpp \
				   ../../../../lantern/src/obj_import.cpp \
				   ../../../../lantern/src/pipeline.cpp \
				   ../../../../lantern/src/point.cpp \
				   ../../../../lantern/src/scene.cpp \
				   ../../../../lantern/src/scene_object.cpp \
				   ../../../../lantern/src/vector3.cpp \
				   ../../../../lantern/src/vector4.cpp \
				   ../../../../tests/src/bitmap_painter.cpp \
				   ../../../../tests/src/camera.cpp \
				   ../../../../tests/src/main.cpp \
				   ../../../../tests/src/matrix3x3.cpp \
				   ../../../../tests/src/matrix4x4.cpp \
				   ../../../../tests/src/obj_import.cpp \
				   ../../../../tests/src/vector3.cpp \
				   ../../../../tests/src/vector4.cpp
LOCAL_STATIC_LIBRARIES := googletest_main
include $(BUILD_EXECUTABLE)
#

$(call import-module,third_party/googletest)
