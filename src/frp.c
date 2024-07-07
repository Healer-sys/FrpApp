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

FrpList_t *GetFrpList()
{
    return FrpList;
}

FrpList_t *GetServerForId(int id)
{
    if (FrpList == NULL) {
        return NULL; // 或者考虑是否需要其他错误处理
    }
    FrpList_t* P = FrpList->next;
    while(P != NULL)
    {
        if(P->id == id) return P;
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
    //服务器列表
    struct json_object *j_GetServerList = json_tokener_parse( get_url("https://api.locyanfrp.cn/Proxies/GetServerList") );
    //printf("%s",json_object_get_string(j_GetServerList));
    if (j_GetServerList == NULL) {
        return 0;
    }

    int ListLength = json_object_array_length(j_GetServerList);

    for(int i = 0; i < ListLength; i++) {

        FrpList_t* new_server = InitFrpList();

        json_object *temp = json_object_array_get_idx(j_GetServerList, i);
        int id = json_object_get_int(json_object_object_get(temp, "id"));
        
        new_server->id = id;
        new_server->name = get_json_string(temp, "name");
        new_server->hostname = get_json_string(temp, "hostname");
        new_server->ip = get_json_string(temp, "ip");
        new_server->status = get_json_string(temp, "status");
        new_server->description = get_json_string(temp, "description");
        
        new_server->next = P->next;
        P->next = new_server;

        // printf("%d %s %s %s %s %s\n", id, new_server->name, new_server->hostname, new_server->ip, new_server->status, new_server->description);
        // printf("%s\n", json_object_get_string(temp));
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
    FrpList_t* P = FrpList->next;
    while(P != NULL)
    {
        printf("%d %s %s %s %s %s\n", P->id, P->name, P->hostname, P->ip, P->status, P->description);
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
    FrpList_t* P = FrpList;
    while(FrpList != NULL)
    {
        P = FrpList->next;
        free(FrpList);
        FrpList = P;
    }
}
