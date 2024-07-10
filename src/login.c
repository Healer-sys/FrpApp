#include "login.h"

char login(UserData_t *user)
{
    if (!user || !user->username || !user->password) {
        printf("用户数据无效\n");
        return 0;
    }

    char url[200] = {0};
    // 使用snprintf避免溢出，确保URL长度不超过数组大小
    snprintf(url, sizeof(url), "https://api.locyanfrp.cn/User/DoLogin?username=%s&password=%s", user->username, user->password);
    #ifdef DEBUG
    printf("[Login URL : %s]\n",url);
    #endif
    char *login_data = (char*)get_url(url);
    if (!login_data) {
        printf("获取URL数据失败\n");
        return 0;
    }

    struct json_object *j_login = json_tokener_parse(login_data);
    if (!j_login) {
        printf("JSON解析失败\n");
        free(login_data);  // 释放login_data内存
        return 0;
    }

    struct json_object *status = NULL;
    if (!json_object_object_get_ex(j_login, "status", &status)) {
        printf("获取status失败\n");
        json_object_put(j_login);  // 释放j_login内存
        free(login_data);          // 释放login_data内存
        return 0;
    }

    int status_code = json_object_get_int(status);
    if (status_code >= 0) {
        // 保存数据
        #ifdef DEBUG
        printf("正在保存数据 -> %s\n",login_data);
        #endif
        HandleUserData(login_data, user);
        printf("登录成功！\n");
    } else {
        printf("账号或密码错误，或者账号不存在\n");
    }

    #ifdef DEBUG
    printf("%s\n",json_object_get_string(j_login));
    #endif

    json_object_put(j_login);  // 释放j_login内存
    free(login_data);          // 释放login_data内存
    return status_code >= 0 ? 1 : 0;
}
