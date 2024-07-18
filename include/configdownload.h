#ifndef _CONFIGDOWNLOAD_H
#define _CONFIGDOWNLOAD_H

#include "user.h"
#include "get.h"
#include "frp.h"
#include "tunnel.h"
/*
* @brief 下载配置文件
* @param user 用户数据
* @param FrpList 服务器列表
* @param Servername 服务器名称
* @param Filename 文件名
*/
void download(UserData_t* user, FrpList_t* FrpList, const char* Servername,const char* Filename);
/*
* @brief 下载所有配置文件
* @param user 用户数据
* @param FrpList 服务器列表
*/
void download_all(UserData_t* user,FrpList_t* FrpList);
/*
* @brief 下载单个配置文件
* @param user 用户数据
* @param FrpList 服务器列表
*/
void download_one(UserData_t* user, FrpList_t* FrpList);
#endif