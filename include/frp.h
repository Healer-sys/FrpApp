#ifndef _FRP_H_
#define _FRP_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "get_request.h"
#include "json-c/json.h"

// 链表节点定义
typedef struct FrpListStruct
{
	int id;
	char *name;
	char *description;
	char *ip;
    char *hostname;
    char *status;
    struct FrpListStruct* next;
}FrpList_t;

// 初始化链表
FrpList_t* InitFrpList();

int get_json_int(json_object *jso, const char *key);
char* get_json_string(json_object *jso, const char *key);

int GetFrpServerList(FrpList_t* frp);

#endif