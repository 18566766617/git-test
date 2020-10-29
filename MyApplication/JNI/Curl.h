//
// Created by Ace on 2017/8/8.
//

#ifndef OBO_170325_CURL_H
#define OBO_170325_CURL_H

#include <curl/curl.h>
#include <string>
using namespace std;


class Curl {

public:
    Curl(string url, bool ignoreCA);

    ~Curl();

    static size_t deal_response(void *ptr, size_t m, size_t n, void *arg);

    //想远程服务器发送请求
    bool execute(string requestData);

    //提供_responseData的getter方法
    string responseData();

private:
    CURL *_curl;
    string _responseData;//用来存储从服务器返回的数据
};


#endif //OBO_170325_CURL_H
