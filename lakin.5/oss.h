#pragma once
#include "config.h"

#ifndef CREATED_STATICS
//
// if statics not created yet do so
//
#define CREATED_STATICS
static unsigned long maxTimeBetweenNewProcsNS = 100;
static int maxTimeBetweenNewProcsSecs = 1;
static int g_bitVector = 0;
static char * perror_arg0 = "oss"; // pointer to return error value

#else
//
// otherwise just refer to them
//
extern unsigned long maxTimeBetweenNewProcsNS = 100;
extern int maxTimeBetweenNewProcsSecs = 1;
extern int g_bitVector = 0;
extern char * perror_arg0 = "oss"; // pointer to return error value

#endif

char perror_buf[50]; // buffer for perror

static int shm_id = -1; // shared memory identifier


//static int msg_id = -1;
//int g_bitVector = 0;
// char logbuf[200];
char indBuf[2];
pid_t pid;
sem_t *semaphore;
time_t startTime;

void initialize();
void initializeSharedMemory();
//void initializeMessageQueue();
struct proc_ctrl_blck  * createProcess();
void requestResponse();
void createMessageQueue();
void launchNewProc();
void ossClock();
// void updateClock(int, int);
// void logger(const char *);
void deinitSharedMemory();
void setBit(int);
bool bitIsSet(int);
void clearBit(int);
void bail();
void sigHandler(const int);
int initializeSig();
void scheduler();
int findAvailablePcb(void);
void initializeSemaphore();

void printStats();
void initStats();
