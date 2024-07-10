#ifndef _CONFIGDOWNLOAD_H
#define _CONFIGDOWNLOAD_H

#include "user.h"
#include "get.h"
#include "frp.h"
#include "tunnel.h"

void download(UserData_t* user, const char* Servername,const char* Filenam);
void download_all(UserData_t* user);
void download_one(UserData_t* user);
#endif