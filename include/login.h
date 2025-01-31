#ifndef _LOGIN_H_
#define _LOGIN_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"
#include "get_request.h"
#include "data.h"
#include "json-c/json.h"
/*
* 登录
* @param user 用户结构体
* @return 1 登录成功 0 登录失败
*/
char login(UserData_t *user);

#endif
