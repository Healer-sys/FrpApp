#ifndef __LOGIN_H
#define __LOGIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"
#include "get.h"
#include "data.h"
#include "json-c/json.h"

char* login(UserData_t *user);
#endif
