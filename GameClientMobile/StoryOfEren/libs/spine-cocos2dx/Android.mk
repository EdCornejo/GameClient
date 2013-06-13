LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := spineCocos2d

LOCAL_MODULE_FILENAME := libspineCocos2d

LOCAL_SRC_FILES := src/spine/spine-cocos2dx.cpp \
src/spine/CCSkeleton.cpp \
src/spine/CCSkeletonAnimation.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/src

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../spine-c/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../cocos2dx
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../cocos2dx/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../cocos2dx/kazmath/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../cocos2dx/platform/android

include $(BUILD_STATIC_LIBRARY)

