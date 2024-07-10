#ifndef _TUNNEL_H_
#define _TUNNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frp.h"
#include "user.h"

/*
* 单个隧道信息
{
    "id": 67120,
    "proxy_name": "modf",
    "proxy_type": "tcp",
    "local_ip": "127.0.0.1",
    "local_port": 22,
    "remote_port": "25805",
    "use_compression": "0",
    "use_encryption": "0",
    "domain": "",
    "node": 20,
    "icp": ""
}
 */


void get_tunnel(UserData_t *user);
void free_tunnellist(Tunnel_t *P);

#endif