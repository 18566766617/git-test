
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

