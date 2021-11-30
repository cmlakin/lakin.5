#include "shm.h"
#include "config.h"
//#include "osclock.h"
#include "resource.h"
#include "deadlock.h"
#include "logger.h"

void checkRequest(int id) {
  shm_data = shmAttach();
  srand(time(0));
  int termChance = 2; //rand() % 10 +1;
  int pInd = id;
  int rInd = shm_data->ptab.pcb[pInd].resReqIndex;
  printf("in checkRequest\n");
  printf("pInd = %i\n", pInd);
  printf("rInd = %i\n", rInd);
  printf("termChance = %i\n", termChance);

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
      }
      else {
        printf("proc terminated\n");
        releaseResources(pInd);
  			kill(getpid(), SIGKILL);
      }

    }
    else { // simulate allocation

      snprintf(logbuf, sizeof(logbuf),
              "Master running deadlock detection at time %0d:%09d:\n", osclock.seconds(), osclock.nanoseconds());
      logger(logbuf);
      // check for safe state
      //if (safe (newstate)) {

        // snprintf(logbuf, sizeof(logbuf), "\tSafe state after granting request\n");
        // logger(logbuf);
        //< carry out allocation >;
        printf("request granted\n");
        // snprintf(logbuf, sizeof(logbuf), "\tMaster granting P%i request R%i at time %0d:%09d\n",
        //           pInd, rInd, osclock.seconds(), osclock.nanoseconds());
        // logger(logbuf);
        //grant request resources
        shm_data->r_state.alloc[pInd][rInd] = shm_data->r_state.alloc[pInd][rInd] + shm_data->ptab.pcb[pInd].request[0];
        //update available resources
        shm_data->r_state.available[rInd] = shm_data->r_state.available[rInd] - shm_data->ptab.pcb[pInd].request[0];
      // }
      // else {
      //   snprintf(logbuf, sizeof(logbuf), "\tUnsafe state after granting request; request not granted\n");
      //   logger(logbuf);
      //
      //   //< restore original state >;
      //   //grant request resources
      //   shm_data->r_state.alloc[pInd][rInd] = shm_data->r_state.alloc[pInd][rInd] - shm_data->ptab.pcb[pInd].request[0];
      //   //update available resources
      //   shm_data->r_state.available[rInd] = shm_data->r_state.available[rInd] + shm_data->ptab.pcb[pInd].request[0];
      //   //< suspend process i>;
      //   printf("put process in wait queue\n");
      // }


    }
  }

}

// void checkRelease(int id) {
//   shm_data = shmAttach();
//   int i;
//   printf("current available:\n");
//   for (i = 0; i < RESOURCES; i++) {
//     printf("%02d ",shm_data->r_state.available[i]);
//   }
//
//   for (i = 0; i < RESOURCES; i++) {
//     shm_data->r_state.available[i] += shm_data->ptab.pcb[id].rsrcsNeeded[i];
//   }
//
//   printf("new available:\n");
//   for (i = 0; i < RESOURCES; i++) {
//     printf("%02d ",shm_data->r_state.available[i]);
//   }
//
// }

// boolean safe (state S) {
// int currentavail[m];
// // not sure exactly what the statement below does.
// //process rest[<number of processes>]; // list of processes running currentavail = available;
// rest = {all processes};
// possible = true;
// while (possible) {
//       //find a process Pk in rest such that
//       claim[k,*] - alloc[k,*] <= currentavail;
//       if (found) {        // simulate execution of Pk
//          currentavail = currentavail + alloc[k,*];
//          rest = rest - {Pk};
//        } else
//          possible = false;
//        }
//    return (rest == null);
// }
