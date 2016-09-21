LOCAL_PATH := $(call my-dir)
#
include $(CLEAR_VARS)

LOCAL_MODULE := lantern
LOCAL_CPPFLAGS := -std=c++11 -O3 -s -DNDEBUG -ffast-math -fexceptions -Wall -Wextra -Werror

# FILE_LIST := $(wildcard $(LOCAL_PATH)/../src/*.cpp))
# LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES := \
	$(LOCAL_PATH)/org_lantern_Lantern.cpp \
	$(LOCAL_PATH)/../src/app.cpp \
	$(LOCAL_PATH)/../src/camera.cpp \
	$(LOCAL_PATH)/../src/color.cpp \
	$(LOCAL_PATH)/../src/geometry_stage.cpp \
	$(LOCAL_PATH)/../src/line.cpp \
	$(LOCAL_PATH)/../src/math_common.cpp \
	$(LOCAL_PATH)/../src/matrix3x3.cpp \
	$(LOCAL_PATH)/../src/matrix4x4.cpp \
	$(LOCAL_PATH)/../src/merging_stage.cpp \
	$(LOCAL_PATH)/../src/mesh.cpp \
	$(LOCAL_PATH)/../src/rasterizing_stage.cpp \
	$(LOCAL_PATH)/../src/renderer.cpp \
	$(LOCAL_PATH)/../src/texture.cpp \
	$(LOCAL_PATH)/../src/ui_element_base.cpp \
	$(LOCAL_PATH)/../src/vector4.cpp \
	$(LOCAL_PATH)/../../examples/empty_app/main.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/../include
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
