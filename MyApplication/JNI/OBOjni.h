//
// Created by 86185 on 2020/10/27.
//

#ifndef MY_APPLICATION_OBOJNI_H
#define MY_APPLICATION_OBOJNI_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//第一步：导入Socket编程的标准库
//这个标准库：linux数据类型(size_t、time_t等等......)
#include<sys/types.h>
//提供socket函数及数据结构
#include<sys/socket.h>
//数据结构(sockaddr_in)
#include<netinet/in.h>
//ip地址的转换函数
#include<arpa/inet.h>
#include <sys/stat.h>


#include <android/log.h>

#include "cJSON.h"
#include <curl/curl.h>
#include "com_example_myapplication_OBOJNI.h"


#include "Json.h"
#include "Curl.h"
#include "Data.h"




#define jniLogTag           "tag"
#define OBO_SERVER_IP       "https://192.168.0.4"
#define OBO_SERVER_PORT     "7777"

#define RESPONSE_DATA_LEN   (4096)
#define TIME_STR_LEN        (64)

#define JNIINFO(fmt, ...)   __android_log_print(ANDROID_LOG_INFO, jniLogTag, fmt, __VA_ARGS__);


#endif
