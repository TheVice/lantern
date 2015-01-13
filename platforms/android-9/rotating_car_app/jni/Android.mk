LOCAL_PATH := $(call my-dir)

#
include $(CLEAR_VARS)
LOCAL_CFLAGS := -std=c++11 -fexceptions
LOCAL_MODULE := lanternrotatingcar
LOCAL_C_INCLUDES += ../../../lantern/include/
LOCAL_SRC_FILES := ../../../../lantern/src/app.cpp \
				   ../../../../lantern/src/bitmap_painter.cpp \
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
				   ../../../../examples/rotating_car_app/main.cpp
LOCAL_LDLIBS := -landroid -llog
LOCAL_STATIC_LIBRARIES := android_native_app_glue
include $(BUILD_SHARED_LIBRARY)
#

$(call import-module,android/native_app_glue)
