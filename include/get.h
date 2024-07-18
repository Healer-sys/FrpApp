#ifndef _GET_H_
#define _GET_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"
#include "frp.h"
#include "json-c/json.h"
#include "get_request.h"

/*
* 获取用户信息
* @param user 用户结构体
* @return 1 成功 0 失败
*/
int GetUserInfo(UserData_t* user);
/*
* 显示服务器列表
* @param user 用户结构体
* @param FrpList 服务器列表
*/
void ShowNode(UserData_t* user, FrpList_t* FrpList);
/*
* 签到
* @param user 用户结构体
*/
void DoSign(UserData_t* user);

#endif