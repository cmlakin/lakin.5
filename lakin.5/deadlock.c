#include "shm.h"
#include "config.h"
#include "logger.h"
#include "resource.h"
#include "deadlock.h"


void checkRequest(int id) {
  shm_data = shmAttach();
  srand(time(0));
  //int i;
  int termChance = rand() % 10 +1;
  int pInd = id;
  int rInd = shm_data->ptab.pcb[pInd].resReqIndex;
  // printf("in checkRequest\n");
  // printf("pInd = %i\n", pInd);
  // printf("rInd = %i\n", rInd);
  // printf("termChance = %i\n", termChance);

  if (shm_data->r_state.alloc[pInd][rInd] > 0) {
    // resource allocation algorithm
    if (shm_data->r_state.alloc[pInd][rInd] + shm_data->ptab.pcb[pInd].request[0] > shm_data->r_state.claim[pInd][rInd]) {
      // total request > claim
      printf("error: total request > claim\n");
      // error message, request can not
    }
    else if (shm_data->ptab.pcb[pInd].request[0] > shm_data->r_state.available[rInd]) {
      //< suspend process >;
      // put process in wait queue_priority
      if (termChance > PROB_TERMINATE) {
        printf("put process in wait queue\n");
        shm_data->grantWait++;
      }
      else {
        shm_data->procChoseT++;
        procTerminate(pInd);
      }

    }
    else { // simulate allocation

      snprintf(logbuf, sizeof(logbuf),
              "\nMaster running deadlock detection at time %0d:%09d:\n", osclock.seconds(), osclock.nanoseconds());
      logger(logbuf);
      shm_data->numDlckRun++;


      //grant request resources temporarily
      shm_data->r_state.alloc[pInd][rInd] = shm_data->r_state.alloc[pInd][rInd] + shm_data->ptab.pcb[pInd].request[0];
      //update available resources temporarily
      shm_data->r_state.available[rInd] = shm_data->r_state.available[rInd] - shm_data->ptab.pcb[pInd].request[0];

      //check for safe state
      if (safe (shm_data->r_state)) {

        snprintf(logbuf, sizeof(logbuf), "\tSafe state after granting request\n");
        logger(logbuf);
        //< carry out allocation >;
        printf("request granted\n");
        shm_data->grantNow++;
        snprintf(logbuf, sizeof(logbuf), "\tMaster granting P%i request R%i at time %0d:%09d\n",
                  pInd, rInd, osclock.seconds(), osclock.nanoseconds());
        logger(logbuf);
      }
      else {
        snprintf(logbuf, sizeof(logbuf), "\tUnsafe state after granting request; request not granted\n");
        logger(logbuf);
        printDeadlock();
        printf("request denied\n");
        //< restore original state >;
        // request of resources not granted
        shm_data->r_state.alloc[pInd][rInd] = shm_data->r_state.alloc[pInd][rInd] - shm_data->ptab.pcb[pInd].request[0];
        //update available resources
        shm_data->r_state.available[rInd] = shm_data->r_state.available[rInd] + shm_data->ptab.pcb[pInd].request[0];
        //< suspend process i>;
              // put process in wait queue_priority
        if (termChance > PROB_TERMINATE) {
          printf("put process in wait queue\n");
          shm_data->grantWait++;
        }
        else {
        shm_data->procTbyDlck++;
          procTerminate(pInd);
        }
      }
    }
  }

}


// create an array to store index value of processes that cause deadlock
bool safe (state S) {
  int currentavail[20];
  int rest[18];
  int i, k, r;
  int procCount = activeProcs;
  for (i = 0; i < RESOURCES; i++) {
    currentavail[i] = shm_data->r_state.available[i];
  }
  // not sure exactly what the statement below does.
  //process rest[<number of processes>]; // list of processes running currentavail = available;
  for (i = 0; i < activeProcs; i++) {
    rest[i] = shm_data->ptab.pcb[i].local_pid; // ??? not sure if this is right
  }

  int possible = 1;
  while (possible) {

    for (k = 0; k < activeProcs; k++) {
      possible = 1;
      for (r = 0; r < RESOURCES; r++) {
        //find a process Pk in rest such that
        int canGet = shm_data->r_state.work[k][r] <= currentavail[r];
        if (canGet) {        // simulate execution of Pk
           currentavail[r] = currentavail[r] + shm_data->r_state.alloc[k][r];
           // not sure what to do with the line below
          shm_data->procsDlck[r] = 0;
         } else {
           possible = 0;
           // add process index to array
           shm_data->procsDlck[r] = 1;
         }

      }
      if (possible == 1) {
        procCount--;
      }
    }
    return (procCount == 0);
  }
}

void printDeadlock() {
  int i;

  snprintf(logbuf, sizeof(logbuf), "\tProcesses ");
  logger(logbuf);
  for(i = 0; i < RESOURCES; i++) {
    if (shm_data->procsDlck[i] > 0) {
      snprintf(logbuf, sizeof(logbuf), "P%i,", i);
      logger(logbuf);
    }
  }
  snprintf(logbuf, sizeof(logbuf), " deadlocked\n");
  logger(logbuf);
}

void procTerminate(int id){
  int pInd = id;
  int i;

  snprintf(logbuf, sizeof(logbuf), "Process P%i terminated\n", pInd);
  logger(logbuf);
  printf("proc terminated\n");

  snprintf(logbuf, sizeof(logbuf), "\tResources released: ");
  logger(logbuf);
  for(i = 0; i < RESOURCES; i++) {
    if (shm_data->r_state.alloc[pInd][i] > 0) {
      snprintf(logbuf, sizeof(logbuf), "R%i:%i ", i, shm_data->r_state.alloc[pInd][i]);
      logger(logbuf);
    }
  }
  //releaseResources(pInd);
  kill(shm_data->ptab.pcb[pInd].pid, SIGKILL);
}

void checkRelease(int id) {
  shm_data = shmAttach();
  int i;
  printf("current available:\n");
  for (i = 0; i < RESOURCES; i++) {
    printf("%02d ",shm_data->r_state.available[i]);
  }

  for (i = 0; i < RESOURCES; i++) {
    shm_data->r_state.available[i] += shm_data->ptab.pcb[id].rsrcsNeeded[i];
  }

  printf("new available:\n");
  for (i = 0; i < RESOURCES; i++) {
    printf("%02d ",shm_data->r_state.available[i]);
  }

}
