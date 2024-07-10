#include "frp.h"

// 初始化链表
FrpList_t* InitFrpList()
{
    FrpList_t* head = (FrpList_t*)malloc(sizeof(FrpList_t));
    if(head == NULL) {
        fprintf(stderr, "InitFrpList: 内存分配失败\n");
        return NULL;
    }
    head->id = -1;
    head->name = NULL;
    head->description = NULL;
    head->ip = NULL;
    head->status = NULL;
    head->hostname = NULL;
    head->next = NULL;
    return head;
}

char *get_json_string(json_object *jso, const char *key) {
    json_object *jso_value = json_object_object_get(jso, key);
    if (!jso_value || (json_object_get_type(jso_value) != json_type_string)) {
        fprintf(stderr, "get_json_string err: [JSON 对象中没有key或者缺失: key is %s!]\n", key);
        return NULL;
    }
    #ifdef DEBUG
    printf("get_json_string: [%s's jso_value is %s]\n", key, json_object_get_string(jso_value));
    #endif
    return (char *)json_object_get_string(jso_value);
}

int get_json_int(json_object *jso, const char *key) {
    json_object *jso_value = json_object_object_get(jso, key);
    if (!jso_value || (json_object_get_type(jso_value) != json_type_int)) {
        fprintf(stderr, "get_json_int err: [JSON 对象中没有key或者缺失: key is %s!]\n", key);
        return -1;
    }
    #ifdef DEBUG
    printf("get_json_int: [%s's jso_value is %d]\n", key, json_object_get_int(jso_value));
    #endif
    return json_object_get_int(jso_value);
}

FrpList_t *GetFrpList()
{
    return FrpList;
}
// 根据服务器ID获取服务器信息
FrpList_t *GetServerForId(int id)
{
    if (FrpList == NULL) {
        return NULL;
    }
    FrpList_t* P = FrpList->next;
    while(P != NULL)
    {
        if(P->id == id) return P;
        P = P->next;
    }
    return NULL;
}
// 根据服务器名称获取服务器信息
FrpList_t *GetServerForName(const char* Servername) {
    if (FrpList == NULL) {
        return NULL;
    }
    FrpList_t* P = FrpList->next;
    while(P != NULL)
    {
        if( strcmp(P->name, Servername) == 0 ) return P;
        P = P->next;
    }
    return NULL;
}
int GetFrpServerList()
{
    if(FrpList == NULL) {
        FrpList = InitFrpList();
    }
    FrpList_t* P = FrpList;

    struct json_object *j_GetServerList = json_tokener_parse(get_url("https://api.locyanfrp.cn/Proxies/GetServerList"));
    if (j_GetServerList == NULL) {
        fprintf(stderr, "Error: 无法从 URL 解析 JSON\n");
        return 0;
    }

    int ListLength = json_object_array_length(j_GetServerList);

    for(int i = 0; i < ListLength; i++) {
        FrpList_t* new_server = InitFrpList();
        if (new_server == NULL) {
            json_object_put(j_GetServerList);
            return 0;
        }

        json_object *temp = json_object_array_get_idx(j_GetServerList, i);
        new_server->id = get_json_int(temp, "id");

        new_server->name = strdup(get_json_string(temp, "name"));
        new_server->hostname = strdup(get_json_string(temp, "hostname"));
        new_server->ip = strdup(get_json_string(temp, "ip"));
        new_server->status = strdup(get_json_string(temp, "status"));
        new_server->description = strdup(get_json_string(temp, "description"));

        new_server->next = P->next;
        P->next = new_server;

        #ifdef DEBUG
        printf("%d %s %s %s %s %s\n", new_server->id, new_server->name, new_server->hostname, new_server->ip, new_server->status, new_server->description);
        #endif
    }

    json_object_put(j_GetServerList);
    return 1;
}

int UpdateFrpServerList()
{
    free_frp_list();
    if(GetFrpServerList()) {
        return 1;
    }
    return 0;
}

void ShowAllList()
{
    if (FrpList == NULL) {
        return;
    }
    FrpList_t* P = FrpList->next;
    while(P != NULL)
    {
        ShowList(P);
        P = P->next;
    }
}

void ShowList(FrpList_t *P)
{
    if(P == NULL) {
        printf("No server found.\n");
        return;
    }
    printf("%d %s %s %s %s %s\n", P->id, P->name, P->hostname, P->ip, P->status, P->description);
}

void free_frp_list()
{
    FrpList_t* P;
    while(FrpList != NULL)
    {
        P = FrpList->next;
        free(FrpList->name);
        free(FrpList->description);
        free(FrpList->ip);
        free(FrpList->status);
        free(FrpList->hostname);
        free(FrpList);
        FrpList = P;
    }
    #ifdef DEBUG
    printf("Free OK!\n");
    #endif
}
