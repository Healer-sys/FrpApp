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

typedef struct {
	int return_value;
}pthread_return;

// 初始化链表
FrpList_t* InitFrpList();
static FrpList_t* FrpList = NULL;

int get_json_int(json_object *jso, const char *key);
char* get_json_string(json_object *jso, const char *key);

FrpList_t* GetFrpList();
FrpList_t* GetServerForId(int id);
FrpList_t* GetServerForName(const char* Servername);
// 获取服务器列表
void* GetFrpServerList();
// 更新服务器列表
int UpdateFrpServerList();
// 显示服务器列表
void ShowAllList();
void ShowList(FrpList_t* P);
void free_frp_list();

#endif