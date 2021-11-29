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
    //pcb = &shm_data->ptab.pcb[id];
    //printf("pcb local pid: %i\n", pcb->local_pid);
    printf("id= %i ", id);
    uprocInitialize();
    attachSharedMemory();
    printf("\nbefore request\n");
    requestResources();
    printf("\nafter request\n");

    sem_unlink(SEM_NAME);
    return 0;
}

void requestResources() {
  printf("\nin request\n");
  int i;
  for (i = 0; i < RESOURCES; i ++) {
    shm_data->ptab.pcb[id].request[i] = shm_data->r_state.work[id][i];
    printf("%02d ", shm_data->ptab.pcb[id].request[i]);
  }
  printf("\nbefore request print\n");
  printf("\nresources requested:\n");
  // for (i = 0; i < RESOURCES; i++) {
  //   printf("%02d ", request[i]);
  // }
  printf("\nend of request\n");
  printf("id = %i\n", id);
  checkRequest(id);
}

void releaseResources() {

}

void uprocInitialize(){
  sem_t *semaphore = sem_open(SEM_NAME, O_RDWR);

  if (semaphore == SEM_FAILED) {
    perror("sem_open(3) failed\n");
    exit(EXIT_FAILURE);
  }

  if (sem_wait(semaphore) < 0) {
    perror("sem_wait(3) failed on child\n");
    //continue;
  }

  printf("PID %ld aquired semaphore\n", (long) getpid());

  if (sem_post(semaphore) < 0) {
    perror("sem_post(3) error on child");
  }

  sleep(1);

  if (sem_close(semaphore) < 0) {
    perror("sem_close(3) failed\n");
  }

}

void attachSharedMemory() {
    key_t fkey = ftok(FTOK_BASE, FTOK_SHM);

    shm_id = shmget(fkey, sizeof(struct shared_data), 0666 | IPC_CREAT);

    if(shm_id == -1) {
        snprintf(perror_buf, sizeof(perror_buf), "%s: shmget: ", perror_arg1);
        perror(perror_buf);
        return;
    }

    shm_data = (struct shared_data*)shmat(shm_id, NULL, 0);
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
