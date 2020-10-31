# 设置LOCAL_PATH变量
LOCAL_PATH:=$(call my-dir)

##### libcurl.so #########
# 清除变量
include $(CLEAR_VARS)
# 编译的目标
LOCAL_MODULE := libcurl
# 参与编译的文件
LOCAL_SRC_FILES := libcurl.a
include $(PREBUILT_STATIC_LIBRARY)


####### OBOjni.so ##########
# 清除变量
include $(CLEAR_VARS)
# 编译目标的类型 libOBOjni.so
# 编译的目标
LOCAL_MODULE := OBOjni
# 参与编译的文件
LOCAL_SRC_FILES :=test.cpp login.cpp cJSON.cpp Json.cpp reg.cpp Curl.cpp Data.cpp
# 添加其他动态库
LOCAL_LDLIBS := -llog -lz
# 添加依赖的静态库
LOCAL_STATIC_LIBRARIES := libcurl
include $(BUILD_SHARED_LIBRARY)
