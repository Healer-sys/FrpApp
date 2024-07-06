#include "frp.h"

// 初始化链表
FrpList_t* InitFrpList()
{
    FrpList_t* head = (FrpList_t*)malloc(sizeof(FrpList_t));
    if(head == NULL) {
        // printf("err\n");
        return NULL;
    }
    head->next = NULL;
    return head;
}

char *get_json_string(json_object *jso, const char *key) {
    json_object *jso_value = json_object_object_get(jso, key);
    if (!jso_value || (json_object_get_type(jso_value) != json_type_string)) {
        fprintf(stderr, "Error: Missing or invalid JSON object for key: %s\n", key);
        return NULL;
    }
    return json_object_get_string(jso_value);
}

int get_json_int(json_object *jso, const char *key) {
    json_object *jso_value = json_object_object_get(jso, key);
    if (!jso_value || (json_object_get_type(jso_value) != json_type_int)) {
        fprintf(stderr, "Error: Missing or invalid JSON integer for key: %s\n", key);
        return -1;
    }
    return json_object_get_int(jso_value);
}

int GetFrpServerList(FrpList_t *frp)
{
    //服务器列表
    struct json_object *j_GetServerList = json_tokener_parse( get_url("https://api.locyanfrp.cn/Proxies/GetServerList") );
    //printf("%s",json_object_get_string(j_GetServerList));
    if (j_GetServerList == NULL) {
        return 0;
    }

    int ListLength = json_object_array_length(j_GetServerList);
    for(int i = 0; i < ListLength; i++) {
        json_object *temp = json_object_array_get_idx(j_GetServerList, i);

        int id = json_object_get_int(json_object_object_get(temp, "id"));
        frp[id].id = id;
        frp[id].name = get_json_string(temp, "name");
        frp[id].description = get_json_string(temp, "description");
        frp[id].ip = get_json_string(temp, "ip");
        frp[id].hostname = get_json_string(temp, "hostname");
        frp[id].status = get_json_string(temp, "status");
        // printf("%s\n", json_object_get_string(temp));
    }
    json_object_put(j_GetServerList);
    return 1;
}
