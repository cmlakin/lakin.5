#include "shm.h"
#include "config.h"
//#include "oss.h"
#include "resource.h"
#include "deadlock.h"

void checkRequest(int id) {
  shm_data = shmAttach();

  int pInd = id;
  int rInd = shm_data->ptab.pcb[pInd].resReqIndex;
  printf("in checkRequest\n");
  printf("pInd = %i\n", pInd);
  printf("rInd = %i\n", rInd);


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
      printf("put process in wait queue\n");
    }
    else { // simulate allocation
      // check for safe state
      //if (safe (newstate)) {
        //< carry out allocation >;
        printf("request granted\n");
        //grant request resources
        shm_data->r_state.alloc[pInd][rInd] = shm_data->r_state.alloc[pInd][rInd] + shm_data->ptab.pcb[pInd].request[0];
        //update available resources
        shm_data->r_state.available[rInd] = shm_data->r_state.available[rInd] - shm_data->ptab.pcb[pInd].request[0];
      // }
      // else {
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
