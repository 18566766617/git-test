#include "OBOjni.h"

#define RESPONSE_DATA_LEN 4096
//用来接收服务器一个buffer
typedef struct login_response_data {
    login_response_data() {
        memset(data, 0, RESPONSE_DATA_LEN);
        data_len = 0;
    }

    char data[RESPONSE_DATA_LEN];
    int data_len;

} response_data_t;

//处理从服务器返回的数据，将数据拷贝到arg中
size_t deal_response(void *ptr, size_t n, size_t m, void *arg) {
    int count = m * n;

    response_data_t *response_data = (response_data_t *) arg;

    memcpy(response_data->data, ptr, count);

    response_data->data_len = count;

    return response_data->data_len;
}


JNIEXPORT jboolean JNICALL Java_com_example_myapplication_OBOJNI_login
        (JNIEnv *env, jobject obj, jstring j_username, jstring j_passwd, jboolean j_isDriver) {

    const char *username = env->GetStringUTFChars(j_username, NULL);
    const char *passwd = env->GetStringUTFChars(j_passwd, NULL);
    const char *isDriver = j_isDriver == JNI_TRUE ? "yes" : "no";
    __android_log_print(ANDROID_LOG_ERROR, TAG,
                        "JNI-login: username = %s, passwd = %s, isDriver = %s",
                        username, passwd, isDriver);


    char *post_str = NULL;
    CURL *curl = NULL;
    CURLcode res;
    response_data_t responseData;

    //初始化curl句柄
    curl = curl_easy_init();
    if (curl == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "JNI-login: curl init error \n");
        return JNI_FALSE;
    }




    //（1）封装一个json字符串
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
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "username", username);
    cJSON_AddStringToObject(root, "password", passwd);
    cJSON_AddStringToObject(root, "driver", isDriver);
    post_str = cJSON_Print(root);
    cJSON_Delete(root);
    root = NULL;
    __android_log_print(ANDROID_LOG_ERROR, TAG, "JNI-login: post_str = [%s]\n", post_str);

    //(2) 想web服务器 发送http请求 其中post数据 json字符串
    //1 设置curl url
    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.0.4:7777/login");
    //2 开启post请求开关
    curl_easy_setopt(curl, CURLOPT_POST, true);
    //3 添加post数据
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_str);

    //4 设定一个处理服务器响应的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, deal_response);

    //5 给回调函数传递一个形参
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

    //6 向服务器发送请求,等待服务器的响应
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "JNI-login:perform ERROR, rescode= [%d]\n",
                            res);
        return JNI_FALSE;

    }
    //（3）处理服务器响应的数据 此刻的responseData就是从服务器获取的数据


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
    root = cJSON_Parse(responseData.data);

    cJSON *result = cJSON_GetObjectItem(root, "result");
    if (result && strcmp(result->valuestring, "ok") == 0) {
        //登陆成功
        __android_log_print(ANDROID_LOG_ERROR, TAG, "JNI-login:login succ！！！");
        return JNI_TRUE;

    } else {
        //登陆失败
        cJSON *reason = cJSON_GetObjectItem(root, "reason");
        if (reason) {
            //已知错误
            __android_log_print(ANDROID_LOG_ERROR, TAG, "JNI-login:login error, reason = %s！！！",
                                reason->valuestring);

        } else {
            //未知的错误
            __android_log_print(ANDROID_LOG_ERROR, TAG,
                                "JNI-login:login error, reason = Unknow！！！");

        }

        return JNI_FALSE;
    }

}


JNIEXPORT jboolean JNICALL Java_com_example_myapplication_OBOJNI_mylogin
        (JNIEnv *env, jobject obj, jstring j_username, jstring j_passwd, jboolean j_isDriver) {
    const char *username = env->GetStringUTFChars(j_username, NULL);
    const char *passwd = env->GetStringUTFChars(j_passwd, NULL);
    const char *isDriver = j_isDriver == JNI_TRUE ? "yes" : "no";

    char *post_str = NULL;

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
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "username", username);
    cJSON_AddStringToObject(root, "password", passwd);
    cJSON_AddStringToObject(root, "driver", isDriver);
    post_str = cJSON_Print(root);
    __android_log_print(ANDROID_LOG_ERROR, TAG, "JNI-login: post_str = [%s]\n", post_str);
    //(2) 想web服务器 发送http请求 其中post数据 json字符串
    // 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    __android_log_print(ANDROID_LOG_ERROR, TAG, "JNI-login: fd= [%d]\n", fd);

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


            __android_log_print(ANDROID_LOG_ERROR, TAG,
                                "read error");

            exit(1);
        } else if (len == 0) {

            __android_log_print(ANDROID_LOG_ERROR, TAG,
                                "服务器端关闭了连接\n");

            break;
        } else {

            __android_log_print(ANDROID_LOG_ERROR, TAG,
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

