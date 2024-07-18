#ifndef _FRP_H_
#define _FRP_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
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
	int result;
}pthread_return;

// 初始化链表
FrpList_t* InitFrpList();

int get_json_int(json_object *jso, const char *key);
char* get_json_string(json_object *jso, const char *key);

/*
 * 通过id获取服务器列表
 * @param FrpList 服务器列表
 * @param id 服务器id
 * @return 服务器信息
 */
FrpList_t* GetServerForId(FrpList_t* FrpList, int id);
/*
* 通过服务器名字获取服务器列表
* @param FrpList 服务器列表
* @param Servername 服务器名字
* @return 服务器信息
*/
FrpList_t* GetServerForName(FrpList_t* FrpList, const char* Servername);
/*
* 获取服务器列表
* @param FrpList 服务器列表
* @return void* 1 成功 0 失败(void* 使用多线程的pthread_exit 返回)
*/ 
void* GetFrpServerList(void* arg);
/*
* 更新服务器列表
* @param FrpList 服务器列表
* @return 1 成功 0 失败
*/
int UpdateFrpServerList(FrpList_t* FrpList);
// 显示服务器列表
/*
* 显示服务器列表
* @param FrpList 服务器列表
* @return void
*/
void ShowAllList(FrpList_t* FrpList);
/*
* 显示服务器列表
* @param FrpList 服务器列表
* @return void
*/
void ShowList(FrpList_t* FrpList);
/*
* 释放服务器列表
* @param FrpList 服务器列表
* @return void
*/
void free_frp_list(FrpList_t* FrpList);

#endif