#include "configdownload.h"
// 下载配置文件
void download(UserData_t* user, const char* Servername,const char* Filename) {
    if (user == NULL || user->tunnel == NULL || Servername == NULL || Filename == NULL) {
        fprintf(stderr, "参数无效\n");
        return;
    }

    FILE *fd = fopen(Filename, "w");
    if (fd == NULL) {
        perror("无法打开文件 me.ini 进行写入");
        return;
    }

    int count = 0;
    Tunnel_t *P = user->tunnel->next;
    FrpList_t* Server = GetServerForName(Servername);
    if (Server == NULL) {
        fprintf(stderr, "未找到服务器: %s\n", Servername);
        fclose(fd);
        return;
    }

    fprintf(fd,
        "[common]\n"
        "server_addr = %s\n"
        "server_port = 2333\n"
        "tcp_mux = true\n"
        "protocol = tcp\n"
        "user = %s\n"
        "token = LoCyanToken\n"
        "dns_server = 223.6.6.6\n"
        "tls_enable = false\n\n",
        Server->hostname,
        user->frptoken
    );

    while (P != NULL) {
        if (P->node == Server->id) {
            ++count;
            fprintf(fd,
                "[%s]\n"
                "privilege_mode = true\n"
                "type = %s\n"
                "local_ip = %s\n"
                "local_port = %d\n"
                "remote_port = %s\n\n",
                P->proxy_name, P->proxy_type, P->local_ip, P->local_port, P->remote_port);
            #ifdef DEBUG
            // 同时输出到控制台
            printf("***************************************************************************************************************\n");
            printf("*隧道->%d\n", count);
            printf("*隧道名字:%s\n", P->proxy_name);
            printf("*隧道id:%d\n", P->id);
            printf("*隧道类型:%s\n", P->proxy_type);
            printf("*服务器介绍:%s\n", Server->description);
            printf("*链接地址:%s\n", Server->hostname);
            printf("***************************************************************************************************************\n");
            #endif
        }
        P = P->next;
    }

    if (count == 0) {
        printf("*没有找到隧道\n");
        fprintf(fd, "*没有找到隧道\n");
    }

    fclose(fd);
}

void download_all(UserData_t* user) {
    if (user == NULL ) {
        printf("[download_all : 用户 is NULL]\n");
        return;
    }
    if ( GetFrpList() == NULL ) {
        printf("[download_all : FrpList_t is NULL]\n");
        return;
    }

    // 初始化hash数组为0，避免未初始化访问
    char hash[100] = {0};
    Tunnel_t* TunnelTemp = user->tunnel->next;
    FrpList_t* FrpListTemp = GetFrpList()->next;
    char Filename[35];
    //遍历服务器
    while(FrpListTemp != NULL)
    {

        Tunnel_t* TunnelTemp = user->tunnel->next;
        //二层遍历隧道
        while(TunnelTemp != NULL) {
            //如果服务器id和隧道id相同，则下载隧道配置
            if( FrpListTemp->id == TunnelTemp->node && hash[TunnelTemp->node] == 0 ) {
                //设置文件名
                hash[TunnelTemp->node] = 1;
                sprintf(Filename, "%s.ini", FrpListTemp->name);
                download(user, FrpListTemp->name, Filename);
            }
            TunnelTemp = TunnelTemp->next;
        }


        //服务器下移
        FrpListTemp = FrpListTemp->next;
    }
    printf("[下载完成!]\n");
}

void download_one(UserData_t* user) {
    // 清屏
    #ifdef _WIN32
        system("cls"); // 在 Windows 上使用 cls 命令
    #else
        system("clear"); // 在类 Unix 系统上使用 clear 命令
    #endif
    // 检查用户数据是否有效
    if (user == NULL) {
        printf("[download_one: 用户数据为空]\n");
        return;
    }

    // 获取服务器列表
    FrpList_t* FrpListTemp = GetFrpList();
    if (FrpListTemp == NULL) {
        printf("[download_one: FrpList_t 为空]\n");
        return;
    }

    // 初始化数组和变量
    char hash[100] = {0};  // 记录已处理的节点ID
    int count = 0;         // 计数器，记录符合条件的服务器数量
    int flag = 0;          // 用户选择的服务器编号
    char* Servername[100]; // 存储符合条件的服务器名字数组

    printf("选择要下载配置的服务器：\n");

    // 遍历服务器列表
    while (FrpListTemp != NULL) {
        Tunnel_t* TunnelTemp = user->tunnel->next;

        // 遍历隧道列表
        while (TunnelTemp != NULL) {
            // 如果服务器ID和隧道ID匹配且该节点未处理过
            if (FrpListTemp->id == TunnelTemp->node && hash[TunnelTemp->node] == 0) {
                printf("*服务器%d：%s\n", ++count, FrpListTemp->name);
                Servername[count] = FrpListTemp->name; // 记录符合条件的服务器名字
                hash[TunnelTemp->node] = 1;            // 标记节点ID已处理
            }
            TunnelTemp = TunnelTemp->next;
        }

        FrpListTemp = FrpListTemp->next; // 指向下一个服务器
    }

    // 提示用户选择要下载配置的服务器
    printf("\n请输入要下载配置的服务器编号：");
    scanf("%d", &flag);

    // 下载选择的服务器配置
    if (flag > 0 && flag <= count) {
        download(user, Servername[flag], "frps.ini");
        printf("[下载完成！]\n");
    } else {
        printf("[错误：无效的选择]\n");
    }
}