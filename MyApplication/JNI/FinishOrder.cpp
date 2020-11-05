//
// Created by 86185 on 2020/11/5.
//

#include "OBOjni.h"


JNIEXPORT jboolean JNICALL Java_com_example_myapplication_OBOJNI_FinishOrder
        (JNIEnv *env, jobject obj){
    /*
    ==== 给服务端的协议 ====
         https://ip:port/finishOrder  [json_data]
    {
        sessionid: "online-user-xxxxxx-xxx-xxx-xxx-xxxxx",
        orderid: "orderid-xxxx-xxx-xxx-xxx-xxxx"
    }
    */
    Json json;


    json.insert("sessionid", Data::getInstance()->getSessionid().c_str());
    json.insert("orderid", Data::getInstance()->getOrderid().c_str());

    string json_str = json.print();

//    string url = OBO_SERVER_IP;
//    url +=":";
//    url +=OBO_SERVER_PORT;
//    url +="/finishOrder";

    char url[50] = {0};
    sprintf(url, "%s:%s/finishOrder", OBO_SERVER_IP, OBO_SERVER_PORT);
    __android_log_print(ANDROID_LOG_ERROR, jniLogTag, "JNI-FinshOrder: uri = [%s]", url);


    Curl curl(url, true);


    if (curl.execute(json_str) == false) {
        JNIINFO("%s", "curl execute error")
        return JNI_FALSE;
    }



    /*
    ====得到服务器响应数据 ====
    //成功
    {
                result: "ok",
                recode: "0",
     }
    //失败
    {
                result: "error",
                recode: "1", //1 代表session失效，需要重新登录
                             //2 代表服务器发生错误
                reason: "why...."
    }
    */
    string response_data = curl.responseData();
    Json json_response;
    json_response.parse(curl.responseData());

    string result = json_response.value("result");
    if (result.length() != 0) {
        if (result == "ok") {
            Data::getInstance()->setOrderid("NONE");
            return JNI_TRUE;

        }
        else {
            JNIINFO("ret error data= %s", response_data.c_str());
            return JNI_FALSE;
        }
    }

    return JNI_TRUE;

}