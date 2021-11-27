#include "shm.h"
#include "config.h"
#include "oss.h"

// resource allocation algorithm
if (alloc[i,*] + request[*] > claim[i,*]) {
  // total request > claim

  // error message, request can not
}
else if (request[*] > available[*]) {
  //< suspend process >;
  // put process in wait queue_priority
}
else { // simulate allocation
  // grant request resources
  alloc[i,*] = alloc[i,*] + request[*];
  // update available resources
  available[*] = available[*] - request[*];
}
if (safe (newstate)) {
  //< carry out allocation >;
}
else {
  //< restore original state >;
  //< suspend process i>;
}

boolean safe (state S) {
int currentavail[m];
// not sure exactly what the statement below does.
//process rest[<number of processes>]; // list of processes running currentavail = available;
rest = {all processes};
possible = true;
while (possible) {
      //find a process Pk in rest such that
      claim[k,*] - alloc[k,*] <= currentavail;
      if (found) {        // simulate execution of Pk
         currentavail = currentavail + alloc[k,*];
         rest = rest - {Pk};
       } else
         possible = false;
       }
   return (rest == null);
}
