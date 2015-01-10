LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := lanterntests

LOCAL_C_INCLUDES += ../../../include/

LOCAL_SRC_FILES := main.cpp ../../../../tests_src/main.cpp ../../../../tests_src/vector3.cpp
LOCAL_LDLIBS := -landroid -llog
LOCAL_STATIC_LIBRARIES := android_native_app_glue googletest_main

include $(BUILD_SHARED_LIBRARY)

#
include $(CLEAR_VARS)
LOCAL_MODULE := runnablelanterntests
LOCAL_C_INCLUDES += ../../../include/
LOCAL_SRC_FILES := ../../../../tests_src/main.cpp ../../../../tests_src/vector3.cpp
LOCAL_STATIC_LIBRARIES := googletest_main
include $(BUILD_EXECUTABLE)
#

$(call import-module,android/native_app_glue)
$(call import-module,third_party/googletest)
