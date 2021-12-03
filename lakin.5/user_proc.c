#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "oss.h"
#include "user_proc.h"
#include "config.h"
#include "shm.h"
#include "deadlock.h"

/***
	-request resource
		-when first created then at random times
	-set bound when to request/release resources
		-generate random number to do this
	-generate request by putting it in shared memory
		-continue to loop and see if resource was granted
		-can't ask for more than MAX
			(request + allocation) <= MAX
	-check to terminate 0-250 milliseconds
		-must run for at least one second before terminating
			-if yes, release all resources
			-if no, request or release
***/

char perror_buf[50];
const char * perror_arg1 = "user_proc";

void attachSharedMemory();

char strbuf[20];
PCB * pcb;

int main (int argc, char ** argv){
    printf("In user_proc %s\n", argv[1]);

    id = atoi(argv[1]);
    //uprocInitialize();
    attachSharedMemory();
    shm_data->ptab.pcb[id].pid = getpid();

    requestResources();
    loop(id);
    //return 0;
}

void requestResources() {
  //printf("\nuproc in request\n");
  srand(time(0));
  int randNum = 18; //rand() % RESOURCES + 1;
  //printf("rand = %i\n", randNum);

  while (shm_data->r_state.work[id][randNum] == 0) {
    if (randNum == RESOURCES) {
      randNum = 0;
    }
    else {
      randNum++;
    }
  }
  //printf("rand = %i\n", randNum);
  shm_data->ptab.pcb[id].resReqIndex = randNum;
  if(shm_data->r_state.work[id][randNum] > 0){
    shm_data->ptab.pcb[id].request[0] = shm_data->r_state.work[id][randNum];
    //printf("%02d ", shm_data->ptab.pcb[id].request[i]);
  }

  printf("request R[%i] = %i\n", randNum, shm_data->ptab.pcb[id].request[0]);


  // printf("\nbefore request print\n");
  // printf("\nresources requested:\n");
  // for (i = 0; i < RESOURCES; i++) {
  //   printf("%02d ", shm_data->ptab.pcb[id].request[i]);
  // }
  // printf("\nend of request\n");
  // printf("id = %i\n", id);
  shm_data->requestFlag = id;
  printf("in user_proc: requestflag = %i\n", shm_data->requestFlag);

}

void loop(int id){
  struct timespec start;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
  //srand(time(0));
  //int randNum = rand() % 250000000000 + 1;
  printf("in loop()\n");
  while(true) {
      struct timespec now;
      long diff;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &now);
    diff = start.tv_nsec - now.tv_nsec;

    if (diff > 1000000000) {
      break;
    }
  }

  srand(time(0));
  int randNum = rand() % 10 + 1;

  if (randNum < PROB_RELEASE) {
    releaseResources(id);
  }
  else if (randNum < PROB_TERMINATE) {
    procTerminate(id);
  }
  else {
    requestResources();
  }

}

void uprocInitialize(){
  // semaphore = sem_open(SEM_NAME, O_RDWR);
  //
  // if (semaphore == SEM_FAILED) {
  //   perror("sem_open(3) failed\n");
  //   exit(EXIT_FAILURE);
  // }

  // if (sem_wait(semaphore) < 0) {
  //   perror("sem_wait(3) failed on child\n");
  //   //continue;
  // }

  //printf("PID %ld aquired semaphore\n", (long) getpid());
  //
  // if (sem_post(semaphore) < 0) {
  //   perror("sem_post(3) error on child");
  // }

  // sleep(1);
  //
  // if (sem_close(semaphore) < 0) {
  //   perror("sem_close(3) failed\n");
  // }

}

void attachSharedMemory() {
  printf("uproc attachSharedMemory\n");
  shm_data = shmAttach();
}


/************** keep if needed for later ***************/

// void doit(int id) {
//     while(1) {
//         ipcmsg msg;
//
//         msg.mtype = msg.ossid;
//
//         // strcpy(msg.mtext, strbuf);
//         // snprintf(&msg.mtext[0],sizeof(msg.mtext), "from %ld",  id);
//         if (msgsnd(msg_id, (void *)&msg, sizeof(msg), 0) == -1) {
//             printf("oss msg not sent");
//         }
//         id = foo;
//
// 				if(msgrcv(msg_id, (void *)&msg, sizeof(ipcmsg), id + 1, 0) == -1) {
//             printf("error receving message\n");
//             exit(-1);
//         }
//     }
// }
//
