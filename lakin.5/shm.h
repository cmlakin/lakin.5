#pragma once
#include "osclock.h"



typedef struct state {

	int resource[19];
	int available[19];
	int claim[17][19];
	int alloc[17][19];

} state;

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

	struct state r_state;

	// report stats

} shared_data;

int shmGet();
shared_data * shmAttach();
void shmDetach();
