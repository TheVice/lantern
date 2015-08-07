LOCAL_PATH := $(call my-dir)

#
include $(CLEAR_VARS)
LOCAL_CFLAGS := -std=c++11 -Wall -Wno-comment -Werror -fexceptions
LOCAL_MODULE := lanternrasterizedtriangle
LOCAL_C_INCLUDES += ../../../android/include/ \
					../../../lantern/include/ \
					../../../lantern/include/math \
					../../../lantern/include/rendering \
					../../../lantern/include/rendering/geometry \
					../../../lantern/include/rendering/rasterizing \
					../../../lantern/include/rendering/shaders
LOCAL_SRC_FILES := ./app_Graphics.cpp \
				   ./app_LaternRasterizedTriangleAppActivity.cpp \
				   ../../../../android/src/androidApp.cpp \
				   ../../../../android/src/gles1x.cpp \
				   ../../../../android/src/graphics.cpp \
				   ../../../../android/src/internalApp.cpp \
				   ../../../../android/src/misc.cpp \
				   ../../../../lantern/src/app.cpp \
				   ../../../../lantern/src/math/line.cpp \
				   ../../../../lantern/src/math/math_common.cpp \
				   ../../../../lantern/src/math/matrix3x3.cpp \
				   ../../../../lantern/src/math/matrix4x4.cpp \
				   ../../../../lantern/src/math/vector4.cpp \
				   ../../../../lantern/src/rendering/camera.cpp \
				   ../../../../lantern/src/rendering/color.cpp \
				   ../../../../lantern/src/rendering/pipeline.cpp \
				   ../../../../lantern/src/rendering/texture.cpp \
				   ../../../../lantern/src/rendering/geometry/mesh.cpp \
				   ../../../../lantern/src/rendering/geometry/obj_import.cpp \
				   ../../../../lantern/src/rendering/rasterizing/rasterizer_traversal_common.cpp \
				   ../../../../examples/rasterized_triangle_app/main.cpp \
				   $(PICO_PNG_CPP)
LOCAL_LDLIBS := -llog -lGLESv1_CM
include $(BUILD_SHARED_LIBRARY)
#
