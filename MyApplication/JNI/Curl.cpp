//
// Created by Ace on 2017/8/8.
//
#include <android/log.h>
#include "Curl.h"


Curl::Curl(string url, bool ignoreCA) {
    this->_curl = curl_easy_init();
    //设置url路径 给curl句柄
    curl_easy_setopt(this->_curl, CURLOPT_URL, url.c_str());

    if (ignoreCA == true) {
        curl_easy_setopt(this->_curl, CURLOPT_SSL_VERIFYHOST, false);
        curl_easy_setopt(this->_curl, CURLOPT_SSL_VERIFYPEER, false);
    }

}

Curl::~Curl() {
    curl_easy_cleanup(this->_curl);
}

string Curl::responseData() {
    return this->_responseData;
}


size_t Curl::deal_response(void *ptr, size_t m, size_t n, void *arg) {
    Curl *This = (Curl *) arg;

    char *p = (char *) ptr;
    int count = m * n;

    /*
    for(int i = 0; i < count;i++) {
        This->_responseData += p[i];
    }
    */

    copy(p, p + count, back_inserter(This->_responseData));

    return count;
}


//想远程服务器发送请求
bool Curl::execute(string requestData) {
    CURLcode res;


    //设置post请求
    curl_easy_setopt(this->_curl, CURLOPT_POST, true);

    //设置传递的post数据
    curl_easy_setopt(this->_curl, CURLOPT_POSTFIELDS, requestData.c_str());


    //设置curl处理服务器数据回调函数
    curl_easy_setopt(this->_curl, CURLOPT_WRITEFUNCTION, deal_response);

    curl_easy_setopt(this->_curl, CURLOPT_WRITEDATA, this);


    res = curl_easy_perform(this->_curl);
    if (res != CURLE_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "tag", "curl:perform ERROR, rescode= [%d]\n", res);
        return false;
    }

    return true;
}