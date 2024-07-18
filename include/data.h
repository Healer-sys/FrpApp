#ifndef _DATA_H_
#define _DATA_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"
#include "json-c/json.h"
/*
* @brief 处理用户数据
* @param data 登录返回的数据
* @param user 用户结构体
*/
void HandleUserData(char* data, UserData_t *user);

#endif