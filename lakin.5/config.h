#pragma once

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "osclock.h"

#define ALARM_TIME 100

#define FTOK_SHM 1
#define FTOK_MSG 2
#define FTOK_BASE "oss.c"

// #define SEM_NAME "/try48"
// #define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
// #define INITIAL_VALUE 1
// #define CHILD_PROGRAM "user_proc"
// #define ITERS 10

#define MAX_TOT_PROCS 40
#define PROCESSES 18
#define RESOURCES 20
#define LOG_FILENAME "oss.log"

#define PROB_TERMINATE 3		// 30% of the time terminate if request not granted
#define PROB_RELEASE	 2		// 20% of the time CPU bound

#define NEW_PROC_MAX_SECS 2
#define NEW_PROC_MAX_NANO 1000000000

#define MAX_TEXT 50

enum queue_priority {
    QT_HIGH_PRIORITY = 100,
    QT_LOW_PRIORITY = 101
};

static int totalProcesses = 0;
//static struct shared_data * shm_data = NULL;
static int MAX = 10; // maximum number of resources
//static int request[20];
static int testNum = 2;

//static int totProcsCreated = 0;
static int allocatedProcs = 0;


// #define MSG_SEND_UPROC 1
// #define MSG_RECV_UPROC MSG_SEND_UPROC
// #define MSG_SEND_OSS 2
// #define MSG_RECV_OSS MSG_SEND_OSS

// message buffer
// typedef struct ipcmsg {
// 	long mtype;
// 	char mtext[MAX_TEXT];
//
// 	int ossid;
// 	int pRunSec;
// 	int pRunNano;
// 	int pOperation;
//
// } ipcmsg;
