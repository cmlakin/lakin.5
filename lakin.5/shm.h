#pragma once
#include "osclock.h"


//typedef struct state {
//
//	int resource[];
//	int available[];
//	int claim[][];
//	int alloc[][];
//
//};

typedef struct proc_ctrl_blck {
	int pid;
	int local_pid;
	int rsrcsNeeded[19]; // fill randomly
} PCB;

typedef struct proc_table {
	struct proc_ctrl_blck pcb[17];
} proc_table;

// shared memory
typedef struct shared_data {

	int local_pid; 	// assigned PID

	int launchSec;
	int launchNano;
	// simulated clock
   osclock_t osclock;

	// process table
	struct proc_table ptab;

	// report stats

} shared_data;

int shmGet();
shared_data * shmAttach();
void shmDetach();
