#include "data.h"
//处理数据

// 处理用户数据的函数
void HandleUserData(char* data, UserData_t *user) {
    FILE *fd = fopen("user.json", "w+");
    if (fd == NULL) {
        fprintf(stderr, "错误：无法打开文件进行写入\n");
        return;
    }

    struct json_object *key = json_tokener_parse(data);
    if (key == NULL) {
        fprintf(stderr, "错误：无法解析JSON数据\n");
        fclose(fd);
        return;
    }

    struct json_object *val = NULL;
    struct json_object *j_userdata_key = json_object_new_object();

    // 处理token
    if (json_object_object_get_ex(key, "token", &val)) {
        user->token = strdup(json_object_get_string(val));
        json_object_object_add(j_userdata_key, "token", json_object_get(val));
    } else {
        fprintf(stderr, "错误：在JSON数据中找不到token键\n");
    }

    struct json_object *j_userdata = NULL;

    // 处理userdata
    if (json_object_object_get_ex(key, "userdata", &j_userdata)) {
        if (json_object_object_get_ex(j_userdata, "username", &val)) {
            user->username = strdup(json_object_get_string(val));
            json_object_object_add(j_userdata_key, "username", json_object_get(val));
        } else {
            fprintf(stderr, "错误：在userdata JSON数据中找不到username键\n");
        }

        if (json_object_object_get_ex(j_userdata, "email", &val)) {
            json_object_object_add(j_userdata_key, "email", json_object_get(val));
        } else {
            fprintf(stderr, "错误：在userdata JSON数据中找不到email键\n");
        }

        if (json_object_object_get_ex(j_userdata, "frptoken", &val)) {
            user->frptoken = strdup(json_object_get_string(val));
            json_object_object_add(j_userdata_key, "frptoken", json_object_get(val));
        } else {
            fprintf(stderr, "错误：在userdata JSON数据中找不到frptoken键\n");
        }
    } else {
        fprintf(stderr, "错误：在JSON数据中找不到userdata键\n");
    }

    const char *json_str = json_object_to_json_string(j_userdata_key);
    fwrite(json_str, 1, strlen(json_str), fd);

    // 清理
    json_object_put(j_userdata_key);
    json_object_put(key);
    fclose(fd);
}