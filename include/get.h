#ifndef __GET_H
#define __GET_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curl/curl.h"
#include "json-c/json.h"

size_t get_data(char *duffer, size_t size, size_t nmemd, void **userp);
const char *get_url(const char *url);
//获取服务器列表并保留自己有的服务器
int GetServerList(const char *username, const char *token, const char *frptoken);
void ShowNode(char *username,char *token);

int id_form_proxy();

#endif