#pragma once
#include "osclock.h"
#include "config.h"



typedef struct state {

	int resource[20];
	int available[20];
	int claim[18][20];
	int alloc[18][20];

} state;

typedef struct proc_ctrl_blck {
	int pid;
	int local_pid;
	int rsrcsNeeded[20]; // fill randomly
} PCB;

typedef struct proc_table {
	struct proc_ctrl_blck pcb[18];
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

	struct state r_state;

	// report stats

} shared_data;

int shmGet();
shared_data * shmAttach();
void shmDetach();
