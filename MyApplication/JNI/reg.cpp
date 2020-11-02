//
// Created by 86185 on 2020/10/29.
//

#include "OBOjni.h"


JNIEXPORT jboolean JNICALL Java_com_example_myapplication_OBOJNI_Reg
        (JNIEnv *env, jobject obj, jstring jusername, jstring jpasswd,
         jstring jtel, jstring jemail, jstring jid_card, jboolean jisDriver) {


    const char *username = env->GetStringUTFChars(jusername, NULL);
    const char *passwd = env->GetStringUTFChars(jpasswd, NULL);
    const char *tel = env->GetStringUTFChars(jtel, NULL);
    const char *email = env->GetStringUTFChars(jemail, NULL);
    const char *id_card = env->GetStringUTFChars(jid_card, NULL);
    const char *isDriver = (jisDriver == JNI_TRUE) ? "yes" : "no";


    __android_log_print(ANDROID_LOG_ERROR, jniLogTag,
                        "REG: username = %s, passwd = %s, tel = %s, email =%s, idcard = %s,isDriver = %s",
                        username, passwd, tel, email, id_card, isDriver);


    //保存当前角色
    Data::getInstance()->setIsDriver(isDriver);
    //设置当前的orderid为空
    Data::getInstance()->setOrderid("NONE");
    //更新当前角色状态
    if (jisDriver == JNI_TRUE) {
        Data::getInstance()->setStatus(OBO_STATUS_DRIVER_IDLE);
    } else {
        Data::getInstance()->setStatus(OBO_STATUS_PASSNGER_IDLE);
    }


    /*
  * 给服务端的协议   https://ip:port/reg [json_data]
  *
 {
     username: "aaa",
     password: "bbb",
     driver:   "yes/no",
     tel:      "13331333333",
     email:    "danbing_at@163.cn",
     id_card:  "2104041222121211122"
 }
  */
    Json json;

    json.insert("username", username);
    json.insert("password", passwd);
    json.insert("driver", isDriver);
    json.insert("tel", tel);
    json.insert("email", email);
    json.insert("id_card", id_card);

    string json_str = json.print();


    //释放字符串资源
    env->ReleaseStringUTFChars(jusername, username);
    env->ReleaseStringUTFChars(jpasswd, passwd);
    env->ReleaseStringUTFChars(jtel, tel);
    env->ReleaseStringUTFChars(jemail, email);
    env->ReleaseStringUTFChars(jid_card, id_card);


//    string url = OBO_SERVER_IP;
//    url +=":";
//    url +=OBO_SERVER_PORT;
//    url +="/reg";

    char url[50] = {0};
    sprintf(url, "%s:%s/reg", OBO_SERVER_IP, OBO_SERVER_PORT);
    __android_log_print(ANDROID_LOG_ERROR, jniLogTag, "JNI-reg: uri = [%s]", url);


    Curl curl(url, true);


    if (curl.execute(json_str) == false) {
        __android_log_print(ANDROID_LOG_ERROR, jniLogTag, "%s", "curl execute error", url);
        return JNI_FALSE;
    }


    /*
      *  得到服务器响应数据, 注册成功就默认为登陆状态

        //成功
        {
             result: "ok",
             sessionid: "xxxxxxxx"
         }
         //失败
         {
             result: "error",
             reason: "why...."
         }

      */

    string response_data = curl.responseData();
    Json json_response;
    json_response.parse(curl.responseData());

    string result = json_response.value("result");
    if (result.length() != 0) {
        if (result == "ok") {

            Data::getInstance()->setSessionid(json_response.value("sessionid"));
            __android_log_print(ANDROID_LOG_ERROR, jniLogTag, "reg succ, sessionid=%s",
                                Data::getInstance()->getSessionid().c_str());
            return JNI_TRUE;

        } else {
            __android_log_print(ANDROID_LOG_ERROR, jniLogTag, "ret error data= %s",
                                response_data.c_str());
            return JNI_FALSE;
        }
    }

    __android_log_print(ANDROID_LOG_ERROR, jniLogTag, "result len =%d",
                        result.length());

    return JNI_FALSE;


}