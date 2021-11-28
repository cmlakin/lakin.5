#include "shm.h"
//#include "config.h"
//#include "oss.h"
#include "resource.h"
#include "deadlock.h"

void checkRequest(int index) {
  int j;

  for (j = 0; j < RESOURCES; j++) {
    // resource allocation algorithm
    if (shm_data->r_state.alloc[index][j] + request[j] > shm_data->r_state.claim[index][j]) {
      // total request > claim
      printf("error: total request > claim\n");
      // error message, request can not
    }
    else if (request[j] > shm_data->r_state.available[j]) {
      //< suspend process >;
      // put process in wait queue_priority
      printf("put process in wait queue\n");
    }
    else { // simulate allocation
      printf("request granted\n");
      // grant request resources
      shm_data->r_state.alloc[index][j] = shm_data->r_state.alloc[index][j] + request[j];
      // update available resources
      shm_data->r_state.available[j] = shm_data->r_state.available[j] - request[j];
    }
  }
}

// if (safe (newstate)) {
//   //< carry out allocation >;
// }
// else {
//   //< restore original state >;
//   //< suspend process i>;
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
