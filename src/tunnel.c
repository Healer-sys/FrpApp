#include "tunnel.h"


Tunnel_t* InitTurnelList()
{
    Tunnel_t* head = (Tunnel_t*)malloc(sizeof(Tunnel_t));
    if(head == NULL) {
        fprintf(stderr, "InitTurnelList: 内存分配失败\n");
        return NULL;
    }

    head->id = -1;
    head->node = -1;
    head->local_port = -1;

    head->domain = NULL;
    head->icp = NULL;
    head->local_ip = NULL;
    head->proxy_name = NULL;
    head->proxy_type = NULL;
    head->remote_port = NULL;
    head->use_compression = NULL;
    head->use_encryption = NULL;

    head->next = NULL;
    return head;
}
//获取隧道信息到链表
void get_tunnel(UserData_t *user) {
    
    char GetTunnel_Url[200]  = {0};

    //拼接用户节点信息url
    sprintf((char *)GetTunnel_Url,"https://api.locyanfrp.cn/Proxies/GetProxiesList?username=%s&token=%s",user->username,user->token);
    #ifdef DEBUG
    printf("get_tunnel->GetTunnel_Url: %s\n",GetTunnel_Url);
    #endif
    struct json_object *j_TunnelList = json_tokener_parse( get_url(GetTunnel_Url) );
    if (j_TunnelList == NULL) {
        fprintf(stderr, "Error parsing JSON j_GetProxiesList\n");
        return;
    }


    if (get_json_int(j_TunnelList, "status") == 0) {
        #ifdef DEBUG
        printf("get_tunnel->message: [%s]\n",get_json_string(j_TunnelList, "message"));
        #endif
    }
    else {
        printf("get_tunnel->message: [%s]\n",get_json_string(j_TunnelList, "message"));
        json_object_put(j_TunnelList);
        return;
    }

    int count = get_json_int(j_TunnelList, "count");
    if ( count && json_object_object_get_ex(j_TunnelList, "proxies", &j_TunnelList) ) {

        #ifdef DEBUG
        printf("get_tunnel->proxies: %s\n",json_object_to_json_string(j_TunnelList));
        printf("[get_tunnel->count: %d]\n",count);
        #endif

        if(user->tunnel == NULL) {
            #ifdef DEBUG
            printf("[get_tunnel-> user->tunnel is NULL! and go to InitTurnelList\n]");
            #endif
            user->tunnel = InitTurnelList();
        }

        Tunnel_t *P = user->tunnel;

        for (int i = 0 ; i < count; i++ ) {

            Tunnel_t *new_turnel = InitTurnelList();
            if (new_turnel == NULL) {
                json_object_put(j_TunnelList);
                return ;
            }

            json_object *temp = json_object_array_get_idx(j_TunnelList, i);

            new_turnel->id                = get_json_int(temp, "id");
            new_turnel->node              = get_json_int(temp, "node");
            new_turnel->local_port        = get_json_int(temp, "local_port");
            new_turnel->proxy_name        = strdup( get_json_string(temp, "proxy_name") );
            new_turnel->proxy_type        = strdup( get_json_string(temp, "proxy_type") );
            new_turnel->remote_port       = strdup( get_json_string(temp, "remote_port") );
            new_turnel->use_compression   = strdup( get_json_string(temp, "use_compression") );
            new_turnel->use_encryption    = strdup( get_json_string(temp, "use_encryption") );
            new_turnel->local_ip          = strdup( get_json_string(temp, "local_ip") );            
            new_turnel->icp               = strdup( get_json_string(temp, "icp") );
            
            #ifdef DEBUG

            if(new_turnel->next != NULL) {

                printf("[get_tunnel-> new_turnel->next is no NULL!]\n");
                
            }
            if(P->next != NULL) {
                printf("[get_tunnel-> P->next is no NULL!]\n");
            }
            printf("[get_tunnel-> id: %d]\n",new_turnel->id);

            #endif
            new_turnel->next = P->next;
            P->next = new_turnel;

            #ifdef DEBUG
            printf("[get_tunnel-> tunnel is %d %d %d %s %s %s %s %s %s]\n",
                                            new_turnel->id,
                                            new_turnel->node,
                                            new_turnel->local_port,
                                            new_turnel->proxy_name,
                                            new_turnel->proxy_type,
                                            new_turnel->remote_port,
                                            new_turnel->use_compression,
                                            new_turnel->use_encryption,
                                            new_turnel->local_ip);
            #endif

        }
        json_object_put(j_TunnelList);
        return;
    }
    else {
        fprintf(stderr, "[get_tunnel err: count = -1 and  proxies is null !]\n");
        json_object_put(j_TunnelList);
        return;
    }

}
void free_tunnellist(Tunnel_t *P) {
    Tunnel_t *temp;
    while(P != NULL)
    {
        temp = P->next;
        free(P->domain);
        free(P->icp);
        free(P->local_ip);
        free(P->proxy_name);
        free(P->proxy_type);
        free(P->remote_port);
        free(P->use_compression);
        free(P->use_encryption);
        free(P);
        P = temp;
    }
    #ifdef DEBUG
    printf("Free OK!\n");
    #endif
}