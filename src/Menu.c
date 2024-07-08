#include "Menu.h"

// 定义一个跨平台的清屏函数
void clearScreen() {
    #ifdef _WIN32
        system("cls"); // 在 Windows 上使用 cls 命令
    #else
        system("clear"); // 在类 Unix 系统上使用 clear 命令
    #endif
}

// 欢迎界面
void welcome() {
    printf("******** 欢迎使用节点管理系统 ********\n");
    fflush(stdout); // 刷新输出缓冲区，确保加载信息即时显示

    for (int i = 0; i < 38; ++i) {
        usleep(20000); // 模拟加载延迟，可以根据实际情况调整
        printf(">");
        fflush(stdout);
    }

    printf("\n");
}

// 获取节点操作
void getNode() {
    // printf("获取节点操作\n");
    // 在这里实现获取节点的具体逻辑
    ShowAllList();
}

// 添加节点操作
void addNode() {
    printf("添加节点操作\n");
    // 在这里实现添加节点的具体逻辑
}

// 运行节点操作
void runNode() {
    printf("运行节点操作\n");
    // 在这里实现运行节点的具体逻辑
}

// 显示主菜单
void displayMainMenu() {
    printf("***** 主菜单 *****\n");
    printf("1. 浏览节点\n");
    printf("2. 添加节点\n");
    printf("3. 下载节点运行节点\n");
    printf("4. 直接退出\n");
    printf("请选择操作（1-4）：");
}

// 显示二级菜单
void displaySubMenu() {
    printf("***** 二级菜单 *****\n");
    printf("1. 操作 1\n");
    printf("2. 操作 2\n");
    printf("3. 返回上级菜单\n");
    printf("4. 直接退出\n");
    printf("请选择操作（1-4）：");
}

void Menu() {
    while (1) {
		clearScreen();// 在每次循环开始时清屏

        displayMainMenu();

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                getNode();
                break;
            case 2:
                addNode();
                break;
            case 3:
                runNode();
                break;
            case 4:
                clearScreen();
                printf("谢谢使用，再见！\n");
                exit(0);// 结束整个程序
            default:
                printf("无效的选择，请重新输入。\n");
        }

        // 二级菜单
        if (choice == 1 || choice == 2 || choice == 3) {
            int subChoice;
            do {
				clearScreen();// 在每次循环开始时清屏
				
                displaySubMenu();
                scanf("%d", &subChoice);

                switch (subChoice) {
                    case 1:
                        printf("执行二级菜单操作 1\n");
                        break;
                    case 2:
                        printf("执行二级菜单操作 2\n");
                        break;
                    case 3:
                        printf("返回上级菜单\n");
                        Menu();
                        break;
                    case 4:
                        clearScreen();
                        printf("谢谢使用，再见！\n");
                        exit(0);// 结束整个程序
                        break;
                    default:
                        printf("无效的选择，请重新输入。\n");
                }
            } while (subChoice != 4);
        }
    }
}