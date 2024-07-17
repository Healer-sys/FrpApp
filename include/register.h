#ifndef _REGISTER_H_
#define _REGISTER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "get_request.h"
#include "json-c/json.h"

typedef struct StructRegister {
    char *username;             // 用户名
    char *password;         // 密码
    char *confirmPassword;  // 确认密码
    char *email;            // 邮箱
    char *verify;           // 邮箱验证码
    char *qq;               // QQ
} Register_t;

/*
* 初始化注册
* @return 注册结构体
*/
Register_t* InitRegister();
/*
* 提交注册
* @param username 用户名
* @param password 密码
* @param confirmPassword 确认密码
* @param email 邮箱
* @param qq QQ
* @return 注册结果*/
int requestRegister(const char *username, const char *password, const char *confirmPassword,
                        const char *email, const char *verify, const char *qq);
/*
* 请求验证码
* @param email 邮箱
*/
void requestCode(const char* email);

#endif