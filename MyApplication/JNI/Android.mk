LOCAL_PATH:=$(call my-dir)


include $(CLEAR_VARS)

#libcurl.a
LOCAL_MODULE := libcurl
LOCAL_SRC_FILES := libcurl.a
include $(PREBUILT_STATIC_LIBRARY)



include $(CLEAR_VARS)

#libtestjni.so
LOCAL_MODULE := testjni
LOCAL_SRC_FILES := test.cpp login.cpp cJSON.cpp Json.cpp reg.cpp Curl.cpp
LOCAL_STATIC_LIBRARIES := libcurl
LOCAL_LDLIBS := -llog -lz


include $(BUILD_SHARED_LIBRARY)