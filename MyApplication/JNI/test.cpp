
#include "OBOjni.h"

void testJni_Hello() {
    __android_log_print(ANDROID_LOG_ERROR, "testjni", "JNI: hello jni");
    return;
}


JNIEXPORT void JNICALL Java_com_example_myapplication_OBOJNI_hello_1jni
        (JNIEnv *env, jobject obj) {

    testJni_Hello();

}

JNIEXPORT void JNICALL Java_com_example_myapplication_OBOJNI_hello_1jni2
        (JNIEnv *env, jobject obj) {

}


JNIEXPORT jint JNICALL Java_com_example_myapplication_OBOJNI_test_1jni_1api
        (JNIEnv *env, jobject obj, jint j_a, jint j_b) {
    int a = (int) j_a;
    int b = (int) j_b;

    __android_log_print(ANDROID_LOG_ERROR, "testjni", "JNI: a = %d, b = %d", a, b);


    int c = 30;

    return c;
}

JNIEXPORT jboolean JNICALL Java_com_example_myapplication_OBOJNI_test_1jni_1api2
        (JNIEnv *evn, jobject obj, jboolean j_bool_a) {

    bool arg_bool = (j_bool_a == JNI_TRUE) ? true : false;

    __android_log_print(ANDROID_LOG_ERROR, "testjni", "JNI: bool = %s",
                        ((arg_bool == true) ? "true" : "false"));


    return (arg_bool == true) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL Java_com_example_myapplication_OBOJNI_test_1jni_1api3
        (JNIEnv *env, jobject obj, jstring j_str1/*"abc"*/, jstring j_str2) {

    //jstring --> char*
    const char *c_str1 = NULL;
    const char *c_str2 = NULL;

    //将java中的字符串转换成char*类型
    c_str1 = env->GetStringUTFChars(j_str1, 0);

    __android_log_print(ANDROID_LOG_ERROR, "testjni", "JNI: c_str1 = %s", c_str1);

    //释放java传递过来jstring里面的在堆上开辟的字符串空间
    env->ReleaseStringUTFChars(j_str1, c_str1);


    c_str2 = env->GetStringUTFChars(j_str2, 0);

    __android_log_print(ANDROID_LOG_ERROR, "testjni", "JNI: c_str2 = %s", c_str2);

    env->ReleaseStringUTFChars(j_str2, c_str2);


    //给java返回一个字符串

    jstring ret_j_string = env->NewStringUTF("JNI return String");

    return ret_j_string;

}


JNIEXPORT void JNICALL Java_com_example_myapplication_OBOJNI_test_1jni_1api4_1array
        (JNIEnv *env, jobject obj, jintArray j_int_array) {

    //获取java中 j_int_array数组的首地址
    jint *pia = env->GetIntArrayElements(j_int_array, NULL);
    //得到数组的长度
    jsize array_len = env->GetArrayLength(j_int_array);

    //使用该pia pia[1] pia[2]
    for (int i = 0; i < array_len; i++) {
        __android_log_print(ANDROID_LOG_ERROR, "testjni", "JNI:array[%d]:%d", i, (int) pia[i]);
    }

    //释放j_int_array数据空间
    env->ReleaseIntArrayElements(j_int_array, pia, 0);
}


JNIEXPORT jboolean JNICALL Java_com_example_myapplication_OBOJNI_mylogin
        (JNIEnv *env, jobject obj, jstring j_username, jstring j_passwd, jboolean j_isDriver) {
    const char *username = env->GetStringUTFChars(j_username, NULL);
    const char *passwd = env->GetStringUTFChars(j_passwd, NULL);
    const char *isDriver = j_isDriver == JNI_TRUE ? "yes" : "no";

    char *post_str = NULL;

    __android_log_print(ANDROID_LOG_ERROR, jniLogTag,
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
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "username", username);
    cJSON_AddStringToObject(root, "password", passwd);
    cJSON_AddStringToObject(root, "driver", isDriver);
    post_str = cJSON_Print(root);
    __android_log_print(ANDROID_LOG_ERROR, jniLogTag, "JNI-login: post_str = [%s]\n", post_str);
    //(2) 想web服务器 发送http请求 其中post数据 json字符串
    // 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    __android_log_print(ANDROID_LOG_ERROR, jniLogTag, "JNI-login: fd= [%d]\n", fd);

    // 连接服务器
    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    //serv.sin_addr.s_addr = inet_addr("192.168.0.4");
    // oserv.sin_addr.s_addr = htonl();
    inet_pton(AF_INET, "192.168.0.4", &serv.sin_addr.s_addr);


    connect(fd, (struct sockaddr *) &serv, sizeof(serv));

    // 通信
    while (1) {
        // 发送数据
        char buf[1024] = {0};
        //memset(buf, 0, sizeof(buf));
        //sprintf(buf, "%s", post_str);
        strcpy(buf, post_str);
        write(fd, buf, strlen(buf));

        // 等待接收数据

        int len = read(fd, buf, sizeof(buf));
        if (len == -1) {


            __android_log_print(ANDROID_LOG_ERROR, jniLogTag,
                                "read error");

            exit(1);
        } else if (len == 0) {

            __android_log_print(ANDROID_LOG_ERROR, jniLogTag,
                                "服务器端关闭了连接\n");

            break;
        } else {

            __android_log_print(ANDROID_LOG_ERROR, jniLogTag,
                                "recv buf: %s\n",
                                buf);

        }
    }


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

    close(fd);
    return JNI_TRUE;
}


