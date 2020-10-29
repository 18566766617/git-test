//
// Created by 86185 on 2020/10/29.
//

#ifndef MY_APPLICATION_JSON_H
#define MY_APPLICATION_JSON_H

#include <stdio.h>
#include "cJSON.h"
#include <string>
using namespace std;


class Json {

public:
    Json();

    ~Json();


    //给json插入一对key value (string)
    void insert(string key, string value);

    //将json Object 转换成字符串
    string print();

    //将json字符串解析成一个json对象
    void parse(string json_str);

    //得到json中的某个key对应的value
    string value(string key);

private:
    //防止json进行拷贝， 出现浅拷贝风险
    Json(const Json &);

    Json &operator=(const Json &);

    cJSON *_root;

};


#endif //MY_APPLICATION_JSON_H
