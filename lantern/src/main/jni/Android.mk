LOCAL_PATH := $(call my-dir)
#
include $(CLEAR_VARS)

LOCAL_MODULE := lantern
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS := -std=c++11 -O3 -s -DNDEBUG -ffast-math -Wall -Wextra -Werror

FILE_LIST := $(wildcard $(LOCAL_PATH)/*.c)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

# empty_app
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../../../examples/empty_app/src/main/jni/*.c)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

FILE_LIST := $(wildcard $(LOCAL_PATH)/../../../../examples/empty_app/src/main/jni/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES += \
	$(LOCAL_PATH)/../../../../examples/empty_app/main.cpp

# rasterized_triangle_app
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../../../examples/rasterized_triangle_app/src/main/jni/*.c)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

FILE_LIST := $(wildcard $(LOCAL_PATH)/../../../../examples/rasterized_triangle_app/src/main/jni/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES += \
	$(LOCAL_PATH)/../../../../examples/rasterized_triangle_app/main.cpp

FILE_LIST := $(wildcard $(LOCAL_PATH)/../../../src/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
	$(LOCAL_PATH)/../../../include \
	$(LOCAL_PATH)/../../../../examples/empty_app/src/main/jni \
	$(LOCAL_PATH)/../../../../examples/rasterized_triangle_app/src/main/jni
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
