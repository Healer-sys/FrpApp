#include "get.h"

//获取用户信息
int GetUserInfo(UserData_t* user) {
    
    if (!user || !user->username || !user->token) {
        printf("GetUserInfo:用户数据无效\n");
        return 0;
    }

    char url[200] = {0};
    // 使用snprintf避免溢出，确保URL长度不超过数组大小
    snprintf(url, sizeof(url), "https://api.locyanfrp.cn/Account/info?username=%s&token=%s", user->username, user->token);
    #ifdef DEBUG
    printf("[info URL : %s]\n",url);
    #endif
    char *user_info = (char*)get_url(url);
    if (!user_info) {
        printf("获取URL数据失败\n");
        return 0;
    }

    struct json_object *j_user_info = json_tokener_parse(user_info);
    if (!j_user_info) {
        printf("JSON解析失败\n");
        free(user_info);  // 释放内存
        return 0;
    }

    if(get_json_int(j_user_info,"status") == 0) {
        printf("*************************************************************************\n");
        printf("*Hello, %s 大大\n",get_json_string(j_user_info,"username"));
        printf("*你的邮箱为: %s\n",get_json_string(j_user_info,"email"));
        printf("*密钥(Token): \n");
        printf("*%s\n",get_json_string(j_user_info,"token"));
        printf("*你的剩余流量: %s\n",get_json_string(j_user_info,"traffic"));
        printf("*隧道数: %d\n", get_json_int(j_user_info,"proxies_num"));
        printf("*隧道速率\n");
        printf("*    %d Mbps 上行\n",get_json_int(j_user_info,"inbound") / 128);
        printf("*    %d Mbps 下行\n",get_json_int(j_user_info,"outbound") / 128);
        printf("头像 URL: %s\n", get_json_string(j_user_info,"avatar"));
        printf("*************************************************************************\n");
    }
    else {
        printf("%s\n",get_json_string(j_user_info,"message"));
    }
    free(user_info);
    json_object_put(j_user_info);
    return 1;
}


/*
    显示隧道列表信息
*/
void ShowNode(UserData_t* user)
{
    int count = 0;
    Tunnel_t *P = user->tunnel->next;
    while(P != NULL )
    {
        FrpList_t* Server = GetServerForId(P->node);
        printf("***************************************************************************************************************\n");
        printf("*隧道->%d\n",++count);
        printf("*服务名字:%s\n",Server->name);
        printf("*隧道名字:%s\n",P->proxy_name);
        printf("*隧道id:%d\n",P->id);
        printf("*隧道类型:%s\n", P->proxy_type);
        printf("*服务器介绍:%s\n",Server->description);
        printf("*链接地址:%s\n", Server->hostname);
        printf("***************************************************************************************************************\n");
        P = P->next;
    }
}

void DoSign(UserData_t* user) {
    char Sign_data[50];
    snprintf(Sign_data, sizeof(Sign_data), "token=%s", user->token);
    char* Sign_result = post_url("https://api.locyanfrp.cn/User/DoSign", Sign_data);

    struct json_object *j_Sign_result = json_tokener_parse(Sign_result);
    if (!j_Sign_result) {
        printf("JSON解析失败\n");
        free(Sign_result);  // 释放内存
        return;
    }

    printf("%s", get_json_string(j_Sign_result,"message"));

    free(Sign_result);
    json_object_put(j_Sign_result);
}