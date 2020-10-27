#include "OBOjni.h"


JNIEXPORT jboolean JNICALL Java_com_example_myapplication_OBOJNI_login
        (JNIEnv *env, jobject obj, jstring j_username, jstring j_passwd, jboolean j_isDriver) {

    const char *username = env->GetStringUTFChars(j_username, NULL);
    const char *passwd = env->GetStringUTFChars(j_passwd, NULL);
    const char *isDriver = j_isDriver == JNI_TRUE ? "yes" : "no";


    __android_log_print(ANDROID_LOG_ERROR, TAG,
                        "JNI-login: username = %s, passwd = %s, isDriver = %s",
                        username, passwd, isDriver);

    //封装一个数据协议
    /*

       ====给服务端的协议====
     http://ip:port/login [json_data]
    {
        username: "gailun",
        password: "123123",
        driver:   "yes"
    }
     *
     *
     * */

    //（1）封装一个json字符串

    //(2) 想web服务器 发送http请求 其中post数据 json字符串

    //（3） 等待服务器的响应


    /*

      //成功
    {
        result: "ok",
    }
    //失败
    {
        result: "error",
        reason: "why...."
    }

     *
     * */
    //(4) 解析服务器返回的json字符串

    // 如果“result”字段 == ok,登陆成功， error  登陆失败


    return JNI_TRUE;
}
