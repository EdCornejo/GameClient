LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := spineC

LOCAL_MODULE_FILENAME := libspineC

LOCAL_SRC_FILES := \
src/spine/Animation.c \
src/spine/AnimationState.c \
src/spine/AnimationStateData.c \
src/spine/Atlas.c \
src/spine/AtlasAttachmentLoader.c \
src/spine/Attachment.c \
src/spine/AttachmentLoader.c \
src/spine/Bone.c \
src/spine/BoneData.c \
src/spine/Json.c \
src/spine/RegionAttachment.c \
src/spine/Skeleton.c \
src/spine/SkeletonData.c \
src/spine/SkeletonJson.c \
src/spine/Skin.c \
src/spine/Slot.c \
src/spine/SlotData.c \
src/spine/extension.c 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/src/spine

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/spine

include $(BUILD_STATIC_LIBRARY)
