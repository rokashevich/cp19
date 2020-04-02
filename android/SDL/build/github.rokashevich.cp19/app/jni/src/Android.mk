LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../glm

# Add your application source files here...
LOCAL_SRC_FILES :=  main.cpp gameworld.cpp helpers/maze2d.cpp

LOCAL_SHARED_LIBRARIES := SDL2

LOCAL_LDLIBS := -llog -lEGL -lGLESv3

include $(BUILD_SHARED_LIBRARY)
