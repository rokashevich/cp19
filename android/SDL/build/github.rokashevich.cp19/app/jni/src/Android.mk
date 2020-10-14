LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../glm

LOCAL_SRC_FILES :=  $(wildcard $(LOCAL_PATH)/*.cpp) $(wildcard $(LOCAL_PATH)/helpers/*.cpp)

LOCAL_SHARED_LIBRARIES := SDL2

LOCAL_LDLIBS := -llog -lEGL -lGLESv3

include $(BUILD_SHARED_LIBRARY)
