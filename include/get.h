#ifndef _GET_H_
#define _GET_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"
#include "frp.h"
#include "json-c/json.h"
#include "get_request.h"

//获取服务器列表并保留自己有的服务器
int GetUserInfo(UserData_t* user);
void ShowNode(UserData_t* user);

#endif