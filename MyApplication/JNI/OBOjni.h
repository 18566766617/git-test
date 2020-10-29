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

#define TAG "tag"

#endif
