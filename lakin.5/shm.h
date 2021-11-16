#include "osclock.h"


struct state {

	int resource[];
	int available[];
	int claim[][];
	int alloc[][];

} PCB;

typedef struct proc_table {
	struct state[17];
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

