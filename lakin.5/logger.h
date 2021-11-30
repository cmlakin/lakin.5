#pragma once

#include "config.h"

// #ifndef CREATED_LOG_STATICS
// //
// // if statics not created yet do so
// //
// #define CREATED_LOG_STATICS
// static char * perror_arg0 = "oss"; // pointer to return error value
//
// #else
// //
// // otherwise just refer to them
// //
extern char * perror_arg0 = "oss"; // pointer to return error value

//#endif
char logbuf[200];
char perror_buf[50]; // buffer for perror


void logger(const char *);
