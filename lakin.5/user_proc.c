#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "oss.h"
#include "user_proc.h"
#include "config.h"
#include "shm.h"
#include "deadlock.h"
#include "resource.h"


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
    printf("before request\n");
    requestResources(id);
    //loop(id);
    //return 0;
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
    requestResources(id);
  }
  int randNano = rand() % 500000000;
  updateClock(0, randNano);

}

void attachSharedMemory() {
  printf("uproc attachSharedMemory\n");
  shm_data = shmAttach();
}

// void uprocInitialize(){
//   semaphore = sem_open(SEM_NAME, O_RDWR);
//
//   if (semaphore == SEM_FAILED) {
//     perror("sem_open(3) failed\n");
//     exit(EXIT_FAILURE);
//   }
//
//   if (sem_wait(semaphore) < 0) {
//     perror("sem_wait(3) failed on child\n");
//     //continue;
//   }
//
//   printf("PID %ld aquired semaphore\n", (long) getpid());
//
//   if (sem_post(semaphore) < 0) {
//     perror("sem_post(3) error on child");
//   }
//
//   sleep(1);
//
//   if (sem_close(semaphore) < 0) {
//     perror("sem_close(3) failed\n");
//   }
//
// }
