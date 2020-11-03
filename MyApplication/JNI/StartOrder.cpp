//
// Created by 86185 on 2020/11/3.
//
#include "OBOjni.h"


JNIEXPORT jboolean JNICALL Java_com_example_myapplication_OBOJNI_StartOrder
        (JNIEnv *env, jobject obj,
         jstring jsrc_longitude, jstring jsrc_latitude, jstring jsrc_address,
         jstring jdst_longitude, jstring jdst_latitude, jstring jdst_address,
         jstring jRMB) {


    const char *src_longitude = env->GetStringUTFChars(jsrc_longitude, NULL);
    const char *src_latitude = env->GetStringUTFChars(jsrc_latitude, NULL);
    const char *src_address = env->GetStringUTFChars(jsrc_address, NULL);
    const char *dst_longitude = env->GetStringUTFChars(jdst_longitude, NULL);
    const char *dst_latitude = env->GetStringUTFChars(jdst_latitude, NULL);
    const char *dst_address = env->GetStringUTFChars(jdst_address, NULL);
    const char *RMB = env->GetStringUTFChars(jRMB, NULL);


    Json json;

    json.insert("sessionid", Data::getInstance()->getSessionid().c_str());
    json.insert("driver", "no");
    json.insert("src_longitude", src_longitude);
    json.insert("src_latitude", src_latitude);
    json.insert("dst_longitude", dst_longitude);
    json.insert("dst_latitude", dst_latitude);
    json.insert("src_address", src_address);
    json.insert("dst_address", dst_address);
    json.insert("RMB", RMB);
    char now_time[TIME_STR_LEN] = {0};
    time_t t = time(NULL);
    strftime(now_time, TIME_STR_LEN, "%Y-%m-%d %X", localtime(&t));
    json.insert("create_order_time", now_time);


    string json_str = json.print();
    __android_log_print(ANDROID_LOG_ERROR, jniLogTag, "json_str：%s", json_str.c_str());


    //释放字符串资源
    env->ReleaseStringUTFChars(jsrc_longitude, src_longitude);
    env->ReleaseStringUTFChars(jsrc_latitude, src_latitude);
    env->ReleaseStringUTFChars(jsrc_address, src_address);
    env->ReleaseStringUTFChars(jdst_longitude, dst_longitude);
    env->ReleaseStringUTFChars(jdst_latitude, dst_latitude);
    env->ReleaseStringUTFChars(jdst_address, dst_address);
    env->ReleaseStringUTFChars(jRMB, RMB);


//    string url = OBO_SERVER_IP;
//    url += ":";
//    url += OBO_SERVER_PORT;
//    url += "/startSetOrder";
//
//    char url[50] = {0};
//    sprintf(url, "%s:%s/reg", OBO_SERVER_IP, OBO_SERVER_PORT);
//    __android_log_print(ANDROID_LOG_ERROR, jniLogTag, "JNI-reg: uri = [%s]", url);



//    Curl curl(url, true);


//    if (curl.execute(json_str) == false) {
//        JNIINFO("%s", "curl execute error")
//        return JNI_FALSE;
//    }
    return JNI_FALSE;
}