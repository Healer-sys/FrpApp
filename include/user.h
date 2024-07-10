#ifndef _USER_H_
#define _USER_H_


// 链表隧道定义
typedef struct TUNNEL
{
    int id;
    char *proxy_name;       // 隧道名称
    char *proxy_type;       // 隧道类型
    char *local_ip;         // 本地ip
    int local_port;         // 本地端口
    char *remote_port;      // 远程端口
    char *use_compression;  // 是否压缩
    char *use_encryption;   // 是否加密
    char *domain;           // 域名
    int node;               // 节点id
    char *icp;              // 备注
    struct TUNNEL* next;
}Tunnel_t;

typedef struct UserDataStruct
{
	char *username;
	char *token;
	char *frptoken;
	char *password;
	struct TUNNEL *tunnel;
}UserData_t;

#endif