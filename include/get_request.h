#ifndef _GET_REQUEST_H_
#define _GET_REQUEST_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curl/curl.h"
//get 
size_t get_data(char *duffer, size_t size, size_t nmemd, void *userp);
const char *get_url(const char *url);

#endif