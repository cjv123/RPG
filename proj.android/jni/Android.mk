LOCAL_PATH := $(call my-dir)

LOCAL_MODULE := libmruby      
LOCAL_SRC_FILES :=../libmruby.a 
include $(PREBUILT_STATIC_LIBRARY)    #or PREBUILT_SHARED_LIBRARY

include $(CLEAR_VARS)
LOCAL_MODULE := cocos2dcpp_shared
LOCAL_MODULE_FILENAME := libcocos2dcpp

define walk 
$(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e))) 
endef 
# 遍历Classes目录 
ALLFILES = $(call walk, $(LOCAL_PATH)/../../Classes) 
FILE_LIST := hellocpp/main.cpp 
# 从所有文件中提取出所有.cpp文件 
FILE_LIST += $(filter %.cpp, %.c, $(ALLFILES)) 
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%) 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes $(LOCAL_PATH)/../../mruby/include

LOCAL_STATIC_LIBRARIES:= libmruby
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
