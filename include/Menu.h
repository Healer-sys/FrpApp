#ifndef _MENU_H_
#define _MENU_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "get.h"
#include "frp.h"
#include "user.h"
#include "configdownload.h"
#include "frpdownload.h"

//上下文内容  实现传参
typedef struct StructContext {
    UserData_t* user;
	FrpList_t* srever;
} Context_t;

// 定义菜单节点结构体
typedef struct MenuNode {
    char *title;                    //菜单标题
    int child_count;                //子菜单个数
    void (*function)(Context_t);    //菜单功能实现函数
    struct MenuNode **children;     //子菜单节点
    struct MenuNode *parent;        //父菜单
} MenuNode_t;

void clearScreen();
void* welcome();


// 创建菜单
MenuNode_t* createMenu(const char *title, void (*function)(Context_t));
// 注册菜单
void RegisterMenu(MenuNode_t *parent, MenuNode_t *child);
// 显示菜单
void displayMenu(MenuNode_t *node);
// 处理菜单选项
int processMenu(MenuNode_t **current_node, Context_t context);
// 释放菜单
void freeMenu(MenuNode_t* node);
// 初始化菜单
MenuNode_t* initMenu();

#endif