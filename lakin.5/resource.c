//#include "oss.h"
#include "shm.h"
#include "config.h"
#include "resource.h"

state * initializeResources() {

    shm_data = shmAttach();

    int i, j;
    // initialize all arrays and matrixs with 0's
    for (i = 0; i < PROCESSES; i++){
      shm_data->r_state.resource[i] = shm_data->r_state.available[i] = 0;
      for (j = 0; j < RESOURCES; j++) {
        shm_data->r_state.claim[i][j] = 0;
        shm_data->r_state.alloc[i][j] = 0;
        //shm_data->r_state.work[i][j] = 0;
      }
    }

    // assign system resource values and set initial available values.
    for (i = 0; i < RESOURCES; i++) {
      shm_data->r_state.resource[i] = shm_data->r_state.available[i] = rand() % MAX + 1;
    }
    // print system resources
    // printf("\nSystem Resources:\n");
    // for (i = 0; i < RESOURCES; i++) {
    //     printf("R%02d ", i);
    // }
    // printf("\n");
    // for (i = 0; i < RESOURCES; i++) {
    //      printf(" %02d ", shm_data->r_state.resource[i]);
    // }
    //
    // // print available resources
    // printf("\nAvailable Resources:\n");
    // // print available resources
    // for (i = 0; i < RESOURCES; i++) {
    //     printf("R%02d ", i);
    // }
    // printf("\n");
    // for (i = 0; i < RESOURCES; i++) {
    //      printf(" %02d ", shm_data->r_state.available[i]);
    // }
    // printf("\n");
}
// add process resources to claim matix
state * claimMatrix(PCB *pcb, int pcbIndex) {
    int j;
    printf("\nclaim:");

    for (j = 0; j < 20; j++){
      //printf("resource needed: %02d ", pcb->rsrcsNeeded[j]);
      //printf("\npcb index: %i: ", pcbIndex);
      shm_data->r_state.claim[pcbIndex][j] = pcb->rsrcsNeeded[j];
      printf(" %02d ", shm_data->r_state.claim[pcbIndex][j]);
    }
}

// initialize allocated values when process is added to claim matrix
state * allocMatrix() {
  int i, j;
  printf("\nin alloc\n");

   for (i = 0; i < testNum; i++) {
     for (j = 0; j < 20; j++) {
        if (shm_data->r_state.claim[i][j] > 0) {
          printf("claim[%i] = %i ", j, shm_data->r_state.claim[i][j]);
          shm_data->r_state.alloc[i][j] = rand() % (shm_data->r_state.claim[i][j]) + 1;
          printf("alloc[%i] = %i\n", j, shm_data->r_state.alloc[i][j]);
        }
        else {
          printf("claim[%i] = %i ", j, shm_data->r_state.claim[i][j]);
          printf("alloc[%i] = %i\n", j, shm_data->r_state.alloc[i][j]);
        }
     }
   }
   printf("alloc done\n");
}

// initialize work matrix (claim - alloc)
state * workMatrix(){
  int i, j;

  for (i = 0; i < testNum; i++) {
    for (j = 0; j < RESOURCES; j++) {
      if (shm_data->r_state.claim[i][j] > 0) {
        shm_data->r_state.work[i][j] = shm_data->r_state.claim[i][j] - shm_data->r_state.alloc[i][j];
      }
    }
  }
}

// print claim matrix
void printClaimMatrix() {
    int i, j;
    printf("\nClaim Matrix:\n");
    printf("    ");
    // print available resources
    for (i = 0; i < RESOURCES; i++) {
        printf("R%02d ", i);
    }
    printf("\n");
    for (i = 0; i < testNum; i++) {
      printf("P%02d ", i);
      for(j = 0; j < RESOURCES; j++) {
        printf(" %02d ", shm_data->r_state.claim[i][j]);
      }
      printf("\n");
    }
    printf("\n");
    exit(-1);
}

// print alloc matrix
void printAllocMatrix() {
    int i, j;
    printf("\nAlloc Matrix:\n");
    printf("    ");
    // print available resources
    for (i = 0; i < RESOURCES; i++) {
        printf("R%02d ", i);
    }
    printf("\n");
    for (i = 0; i < testNum; i++) {
      printf("P%02d ", i);
      for(j = 0; j < RESOURCES; j++) {
        printf(" %02d ", shm_data->r_state.alloc[i][j]);
      }
      printf("\n");
    }
    printf("\n");
    exit(-1);
}

// print work matrix
void printWorkMatrix() {
    int i, j;
    printf("\nWork Matrix:\n");
    printf("    ");
    // print available resources
    for (i = 0; i < RESOURCES; i++) {
        printf("R%02d ", i);
    }
    printf("\n");
    for (i = 0; i < testNum; i++) {
      printf("P%02d ", i);
      for(j = 0; j < RESOURCES; j++) {
        printf(" %02d ", shm_data->r_state.work[i][j]);
      }
      printf("\n");
    }
    printf("\n");
    exit(-1);
}
