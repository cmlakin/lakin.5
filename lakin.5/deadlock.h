#pragma once

#include "config.h"
#include "shm.h"
//#include "oss.h"
#include "user_proc.h"


void checkRequest(int);
void checkRelease(int);
void procTerminate(int);
void printDeadlock();
bool safe (state S);
void releaseResources(int);
void requestResources(int);
