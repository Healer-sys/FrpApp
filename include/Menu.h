#ifndef _MENU_H_
#define _MENU_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "frp.h"

void clearScreen();
void welcome();

// 获取节点操作
void getNode();

// 添加节点操作
void addNode();

// 运行节点操作
void runNode();

void displayMainMenu();
void displaySubMenu();
void Menu();

#endif