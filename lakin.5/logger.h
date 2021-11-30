#pragma once

#include "config.h"

char logbuf[200];
char perror_buf[50]; // buffer for perror
char * perror_arg2 = "oss"; // pointer to return error value

void logger(const char *);
