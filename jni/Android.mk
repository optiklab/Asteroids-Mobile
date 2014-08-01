LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LS_CPP=$(subst $(1)/,,$(wildcard $(1)/*.cpp))

#please, replace PATH to actual path, for example: /home/mydebianpc/ANDROID_NDK/sources/freetype-2.5.1/include
LOCAL_CFLAGS    := -D GL_GLEXT_PROTOTYPES -isystem PATH
LOCAL_MODULE    := asteroids
LOCAL_SRC_FILES := $(call LS_CPP,$(LOCAL_PATH))
LOCAL_LDLIBS    := -landroid -llog -lEGL -lGLESv1_CM -lOpenSLES #-pg 

LOCAL_STATIC_LIBRARIES += android_native_app_glue
#LOCAL_STATIC_LIBRARIES += FreeType

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
#$(call import-module,freetype-2.5.1/jni)