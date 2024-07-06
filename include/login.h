#ifndef _LOGIN_H_
#define _LOGIN_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"
#include "get_request.h"
#include "data.h"
#include "json-c/json.h"

char login(UserData_t *user);

#endif
