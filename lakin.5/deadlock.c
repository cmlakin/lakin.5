#include "shm.h"
#include "config.h"
#include "oss.h"

// // resource allocation algorithm
// if (alloc[i,*] + request[*] > claim[i,*]) < error >; // total request > claim
// else if (request[*] > available[*]) < suspend process >;
// else { // simulate allocation
// < define newstate by:
// alloc[i,*] = alloc[i,*] + request[*]; available[*] = available[*] - request[*] >;
// }
// if (safe (newstate) )
// < carry out allocation >; else {
//     < restore original state >;
//     < suspend process i>;
//  }
//
// boolean safe (state S) {
// int currentavail[m];
// process rest[<number of processes>]; // list of processes running currentavail = available;
// rest = {all processes};
// possible = true;
// while (possible) {
//       <find a process Pk in rest such that
//          claim[k,*] - alloc[k,*] <= currentavail;>
//       if (found) {        // simulate execution of Pk
//          currentavail = currentavail + alloc[k,*];
//          rest = rest - {Pk};
//        } else
//          possible = false;
//        }
//    return (rest == null);
// }
