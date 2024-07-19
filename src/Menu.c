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
void* welcome() {
    printf("******** 欢迎使用节点管理系统 ********\n");
    fflush(stdout); // 刷新输出缓冲区，确保加载信息即时显示

    for (int i = 0; i < 38; ++i) {
        usleep(30000); // 模拟加载延迟，可以根据实际情况调整
        printf(">");
        fflush(stdout);
    }

    printf("\n");
    pthread_exit(NULL);
}
// 创建一个新的菜单节点
MenuNode_t* createMenu(const char *title, void (*function)(Context_t)) {
    MenuNode_t *node = (MenuNode_t*)malloc(sizeof(MenuNode_t));
    node->title = strdup(title);
    node->function = function;
    node->children = NULL;
    node->child_count = 0;
    node->parent = NULL;
    return node;
}

void exit_menu(Context_t context) {
    if (context.srever){}
    printf("谢谢使用，再见！\n");
}
void ShowUserInfo_menu(Context_t context) {
    GetUserInfo(context.user);
}
void Check_in_menu(Context_t context) {
    DoSign(context.user);
}
void About_menu(Context_t context) {
    printf("user : %p该项目正在开发中！\n", context.user);
}
void Showtunnellist_menu(Context_t context) {
    ShowNode(context.user, context.srever);
}
void DlconfigOne_menu(Context_t context) {
    download_one(context.user, context.srever);
}
void DlconfigAll_menu(Context_t context) {
    download_all(context.user, context.srever);
}
void Startserver_menu(Context_t context) {
    if (context.user != NULL) {
        // 检查frpc.ini文件是否存在
        // 此处使用了简单的文件存在性检查，实际应用中可能需要更复杂的逻辑
        if(access("./frpc.ini", F_OK) == -1) {
            printf("frpc.ini文件不存在或不可访问！ 请下载配置文件\n");
            return;
        }
        int ret = system("screen -dmS frpc ./frp/frpc -c frpc.ini");
        if(ret == 0){
            printf("启动成功！\n");
        }
        else if (ret == 1) {
            printf("未知错误！\n");
        } 
        else if(ret == -1) {
            printf("创建子进程失败！\n");
        }
        else if(ret == 126) {
            printf("运行时出错！\n");
        }
        else if(ret == 127) {
            printf("命令不可执行! 检查是否安装frp和screen。\n");
        }
    }
}
void addtunnel_menu(Context_t context) {
    printf("user : %p该项目正在开发中！\n", context.user);
    // AddTunnel(context.user);
}
void Dlfrp_menu(Context_t context) {
    
    if( FrpDownload() || context.user == NULL ) {
        printf("下载失败！\n");
    }
    else {
        printf("下载成功！\n");
    }
}
void Serverstatus_menu(Context_t context) {
    ShowAllList(context.srever);
    // printf("user : %p该项目正在开发中！\n", context.user);
    // ServerStatus(context.user);
}
void Document_menu(Context_t context) {
    printf("user : %p该项目正在开发中！\n", context.user);
}
// 添加菜单点到父菜单上
void RegisterMenu(MenuNode_t *parent, MenuNode_t *child) {
    parent->children = (MenuNode_t**)realloc(parent->children, sizeof(MenuNode_t*) * (parent->child_count + 1));
    child->parent = parent;
    parent->children[parent->child_count++] = child;
}

// 显示当前菜单
void displayMenu(MenuNode_t *node) {
    printf("\n--- %s ---\n", node->title);
    for (int i = 0; i < node->child_count; i++) {
        printf("%d. %s\n", i + 1, node->children[i]->title);
    }
    if (node->parent != NULL) {
        printf("%d. 返回上级菜单\n", node->child_count + 1);
    }
    printf("输入你的选择: ");
}

// 处理菜单选择
int processMenu(MenuNode_t **current_node, Context_t context) {
    int choice;
    scanf("%d", &choice);

    if ((*current_node)->parent != NULL) {
        if (choice < 1 || choice > (*current_node)->child_count + 1) {
            printf("无效选择，请重试！\n");
            return 1;
        }
    }
    else {
        if (choice < 1 || choice > (*current_node)->child_count) {
            printf("无效选择，请重试！\n");
            return 1;
        }        
    }

    /*
    * 如果输入值大于子菜单个数，并且存在上级菜单，则返回上级菜单
    * 否则，判断子菜单是否存在函数。
    */
    if (choice == (*current_node)->child_count + 1 && (*current_node)->parent != NULL) {
        *current_node = (*current_node)->parent;
    }
    else {
        MenuNode_t *selected_node = (*current_node)->children[choice - 1];
        if (selected_node->function != NULL) {
            clearScreen();
            selected_node->function(context);
            if (selected_node->function == exit_menu) {
                return 0; // 退出循环
            }
        }
        else {
            *current_node = selected_node;
        }
    }
    return 1;
}

void freeMenu(MenuNode_t* node) {
    for(int i = 0; i < node->child_count; ++i) {
        freeMenu(node->children[i]);
    }
    free(node->title);          // 释放标题，在createMenu里strdup(title);
    free(node->children);       // 释放子节点数组，在createMenu里realloc(children);
    free(node);                 // 释放当前节点，在createMenu里malloc(sizeof(MenuNode_t));
}

MenuNode_t* initMenu() {
    // 创建菜单(无操作函数)
    MenuNode_t *m_root = createMenu("主菜单", NULL);
    MenuNode_t *m_ShowUserInfo = createMenu("显示个人信息", ShowUserInfo_menu);
    MenuNode_t *m_Tunnel_operations = createMenu("隧道操作", NULL);
    MenuNode_t *m_Check_in = createMenu("签到一下吧", Check_in_menu);
    MenuNode_t *m_Other = createMenu("其他功能", NULL);
    MenuNode_t *m_About = createMenu("关于", About_menu);
    MenuNode_t *m_Exit = createMenu("退出", exit_menu);

    // 注册子菜单
    RegisterMenu(m_root, m_ShowUserInfo);
    RegisterMenu(m_root, m_Tunnel_operations);
    RegisterMenu(m_root, m_Check_in);
    RegisterMenu(m_root, m_Other);
    RegisterMenu(m_root, m_About);
    RegisterMenu(m_root, m_Exit);

    // 隧道操作下子菜单
    RegisterMenu(m_Tunnel_operations, createMenu("隧道列表", Showtunnellist_menu));
    RegisterMenu(m_Tunnel_operations, createMenu("下载单个隧道配置文件", DlconfigOne_menu));
    RegisterMenu(m_Tunnel_operations, createMenu("下载所有隧道配置文件", DlconfigAll_menu));
    RegisterMenu(m_Tunnel_operations, createMenu("开启隧道服务", Startserver_menu));
    RegisterMenu(m_Tunnel_operations, createMenu("添加隧道", addtunnel_menu));

    // 其他功能下子菜单
    RegisterMenu(m_Other, createMenu("软件下载", Dlfrp_menu));
    RegisterMenu(m_Other, createMenu("服务器状态", Serverstatus_menu));
    RegisterMenu(m_Other, createMenu("文档", Document_menu));

    return m_root;

    // while (processMenu(&current_node, context)) {
    //     displayMenu(current_node);
    // }
}

