#ifndef _GET_REQUEST_H_
#define _GET_REQUEST_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curl/curl.h"
/*
*/
size_t get_data(char *duffer, size_t size, size_t nmemd, void *userp);
/*
* get请求
* @param url 请求地址
* @return 请求结果
*/
const char *get_url(const char *url);
/*
* post请求
* @param url 请求地址
* @param post_data post数据
* @return 请求结果
*/
char* post_url(const char* url, const char* post_data);

#endif