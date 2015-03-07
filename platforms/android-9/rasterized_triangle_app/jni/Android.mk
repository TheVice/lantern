LOCAL_PATH := $(call my-dir)

#
include $(CLEAR_VARS)
LOCAL_CFLAGS := -std=c++11 -Wall -Wno-comment -Werror -fexceptions
LOCAL_MODULE := lanternrasterizedtriangle
LOCAL_C_INCLUDES += ../../../android/include/ ../../../lantern/include/
LOCAL_SRC_FILES := ../../../../android/src/androidApp.cpp \
				   ../../../../android/src/gles1x.cpp \
				   ../../../../android/src/graphics.cpp \
				   ../../../../android/src/internalApp.cpp \
				   ../../../../android/src/misc.cpp \
				   ../../../../lantern/src/app.cpp \
				   ../../../../lantern/src/camera.cpp \
				   ../../../../lantern/src/color.cpp \
				   ../../../../lantern/src/matrix3x3.cpp \
				   ../../../../lantern/src/matrix4x4.cpp \
				   ../../../../lantern/src/mesh.cpp \
				   ../../../../lantern/src/obj_import.cpp \
				   ../../../../lantern/src/pipeline.cpp \
				   ../../../../lantern/src/texture.cpp \
				   ../../../../lantern/src/vector3.cpp \
				   ../../../../lantern/src/vector4.cpp \
				   ../../../../examples/rasterized_triangle_app/main.cpp
LOCAL_LDLIBS := -landroid -llog -lEGL -lGLESv1_CM
LOCAL_STATIC_LIBRARIES := android_native_app_glue
include $(BUILD_SHARED_LIBRARY)
#

$(call import-module,android/native_app_glue)
