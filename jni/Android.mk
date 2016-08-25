LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := JavaJni
LOCAL_SRC_FILES := JavaJni.c
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
