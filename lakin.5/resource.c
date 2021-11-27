//#include "oss.h"
#include "shm.h"
#include "config.h"
#include "resource.h"

state * initializeResources() {

    shm_data = shmAttach();
    // initialize system resources and available resources
    printf("\nin initialize\n");
    printf("\nMAX = %i\n", MAX);
    int i;
    for (i = 0; i < RESOURCES; i++) {
      shm_data->r_state.resource[i] = shm_data->r_state.available[i] = rand() % MAX + 1;
    }
    // print system resources
    printf("\nSystem Resources:\n");
    for (i = 0; i < RESOURCES; i++) {
        printf("R%02d ", i);
    }
    printf("\n");
    for (i = 0; i < 20; i++) {
         printf(" %02d ", shm_data->r_state.resource[i]);
    }

    // print available resources
    printf("\nAvailable Resources:\n");
    // print available resources
    for (i = 0; i < RESOURCES; i++) {
        printf("R%02d ", i);
    }
    printf("\n");
    for (i = 0; i < RESOURCES; i++) {
         printf(" %02d ", shm_data->r_state.available[i]);
    }
    printf("\n");
}
// add process resources to claim matix
state * claimMatrix(PCB *pcb, int pcbIndex) {
    int j;
    printf("\nin claimMatrix()\n");

    for (j = 0; j < 20; j++){
      //printf("resource needed: %02d ", pcb->rsrcsNeeded[j]);
      //printf("\npcb index: %i: ", pcbIndex);
      shm_data->r_state.claim[pcbIndex][j] = pcb->rsrcsNeeded[j];
      //printf(" claim: %02d\n", shm_data->r_state.claim[pcbIndex][j]);
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
    for (i = 0; i < PROCESSES; i++) {
      printf("P%02d ", i);
      for(j = 0; j < RESOURCES; j++) {
        printf(" %02d ", shm_data->r_state.claim[i][j]);
      }
      printf("\n");
    }
    printf("\n");
    exit(-1);
}
