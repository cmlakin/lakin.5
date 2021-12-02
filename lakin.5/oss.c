#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include <sys/fcntl.h>
#include <stdbool.h>

#include "config.h"
#include "osclock.h"
#include "shm.h"
#include "oss.h"
#include "queue.h"
#include "resource.h"
#include "deadlock.h"
#include "logger.h"

/***
	- allocate shm for data structures
		-create 20 resources with descriptors
			-16 static
			-4 shared (no deadlock detection necessary)
			-initialize with value 1-10
	-18 max processes at one time
	-run deadlock avoidance algorithm before granting resources
		-if yes, update all data structures when resources granted or released
		-if no, put in waiting queue and go to sleep until awakened
	-update clock as necessary
	-fork children at random times (1-500 milliseconds of loggerical clock)

***/
//

int main(int argc, char ** argv){

    unlink(LOG_FILENAME);
    srand(getpid());

    initialize();
    shm_data->requestFlag = -1;

    if (totalProcesses == 0) {
        launchNewProc();
        osclock.add(shm_data->launchSec, shm_data->launchNano);
    }

    while (totalProcesses < testNum) {
      scheduler();
      sleep(1);
    }
    sleep(1);
    // printClaimMatrix();
    // sleep(1);
    //printf("end of oss main\n");
    // printAllocMatrix();
    // sleep(1);
    // printWorkMatrix();
    printStats();

    printf("oss done\n");
    bail();
    return 0;
}

/** maybe reuse **/
void scheduler() {
    PCB * foo;

    foo = createProcess();
    // int pInd = foo->local_pid & 0xff;
    // printf("pInd = %i\n", pInd);

    srand(time(0));
/*  this code is causing problem after execing into user_proc
    // while (totalProcesses < testNum) {
    //
    //
    //   if (activeProcs < testNum) {
    //     int create = osclock.seconds() > shm_data->launchSec;
    //
    //     if(!create && osclock.seconds()) {
    //       create = osclock.seconds() > shm_data->launchSec && osclock.nanoseconds() >= shm_data->launchNano;
    //     }
    //     if(create) {
    //       printf("current %0d:%09d\n", osclock.seconds(), osclock.nanoseconds());
    //       printf("lanuch  %0d:%09d\n", shm_data->launchSec, shm_data->launchNano);
    //       foo = createProcess();
    //       launchNewProc();
    //     }
    //   }
*/

/* tried to implement timer for checking if process should terminate
      srand(getpid());
      int randTermNum = 2;//rand() % 10 + 1;
      int chkTerminate = rand() % 5;
      startTime = time(NULL);
      printf("before while\n");
      while (time(NULL) - startTime < chkTerminate) {
        printf("in while\n");
        if (randTermNum <= PROB_TERMINATE){
          printf("will terminate\n");
          procTerminate(pInd);
          startTime = time(NULL);
        }
        exit(0);
      }
*/


      printf("back in scheduler\n");

      releaseResources(pInd);
    //}
}

PCB * createProcess() {
    printf("\ncreateProcess\n");
    // activeProcs++;
    totalProcesses++;

    PCB *pcb;

    // find available pcb and initialize first values
    int pcbIndex = findAvailablePcb();
    if(pcbIndex == -1) {
        printf("oss: createProcess: no free pcbs\n");
        return NULL;
    }
    //printf("oss: createProcess: available pcb %d\n", pcbIndex);
    setBit(pcbIndex);
    allocatedProcs++;

    pcb = &shm_data->ptab.pcb[pcbIndex];

    pid = pcb->pid = fork();

    if (pid == -1) {
        perror("Failed to create new process\n");
        return NULL;
    } else if (pid == 0) {
        pcb->local_pid = shm_data->local_pid << 8 | pcbIndex;
        srand(time(0));
        int i;
        int max = 0;
        printf("\n");
        for (i = 0; i < RESOURCES; i++) {
             //printf(" %02d ", shm_data->r_state.resource[i]);
             max = shm_data->r_state.resource[i] + 1;
             //printf("max = %02d ", max);
             pcb->rsrcsNeeded[i] = rand() % max;
             //printf("RN%02d = %02d ", i, pcb->rsrcsNeeded[i]);
        }

        claimMatrix(pcb, pcbIndex);

        shm_data->local_pid++;

        snprintf(indBuf, sizeof(indBuf), "%d", pcbIndex);
        if (execl(CHILD_PROGRAM, CHILD_PROGRAM, indBuf, NULL) < 0) {
          perror("execl(2) failed\n");
          exit(EXIT_FAILURE);
        }
    }
    //printf("in oss: requestflag = %i\n", shm_data->requestFlag);
    while(shm_data->requestFlag != pcbIndex) {}
    //printf( "out of while loop\n");

    snprintf(logbuf, sizeof(logbuf),
            "Master has detected Process with PID %i is requesting  R%i at time %0d:%09d\n",
              pcb->local_pid & 0xff, shm_data->ptab.pcb[pcbIndex].resReqIndex,
              osclock.seconds(), osclock.nanoseconds());

    logger(logbuf);

    checkRequest(pcbIndex);
    workMatrix();

    osclock.add(0,1);
    return pcb;
}

void releaseResources(int id) {

  int i;
  for (i = 0; i < RESOURCES; i++) {
    shm_data->r_state.available[i] += shm_data->r_state.alloc[id][i];
    shm_data->r_state.work[id][i] += shm_data->r_state.alloc[id][i];
    shm_data->r_state.alloc[id][i] = 0;
  }
}

void initialize() {
    initializeSharedMemory();
    initializeResources();
    initializeSemaphore();
    initStats();
}

void initStats() {
  shm_data->grantNow = 0;
  shm_data->grantWait = 0;
  shm_data->procTbyDlck = 0;
  shm_data->procChoseT = 0;
  shm_data->numDlckRun = 0;
  shm_data->avgTbyDlck = 0.0;
}
void printStats() {
  printf("Number of times granted resource immediately: %02d\n", shm_data->grantNow);
  printf("Number of times granted resource after waiting: %02d\n", shm_data->grantWait);
  printf("Number of processes terminated by deadlock algorithm: %02d\n", shm_data->procTbyDlck);
  printf("Number of processes who chose to terminate: %02d\n", shm_data->procChoseT);
  printf("Number of times deadlock algorithm ran: %02d\n", shm_data->numDlckRun);
  float avg = shm_data->avgTbyDlck / (float)totalProcesses;
  printf("Average of processes terminated by deadlock algorithm: %.2f%%\n", avg);
}

void initializeSharedMemory() {
    shm_data = shmAttach();
    shm_data->local_pid = 1;
}

void initializeSemaphore() {
  semaphore = sem_open(SEM_NAME, O_CREAT | O_EXCL, SEM_PERMS, INITIAL_VALUE);

  if (semaphore == SEM_FAILED) {
    perror("sem_open(3) error\n");
    exit(EXIT_FAILURE);
  }

  // if (sem_close(semaphore) < 0) {
  //   perror("sem_close(3) failed\n");
  //   sem_unlink(SEM_NAME);
  //   exit(EXIT_FAILURE);
  // }
}

int findAvailablePcb(void) {
    int i;
    //
    // shortcut since bit vector will often be all 1s
    //
    if((g_bitVector & 0x3ffff) == 0x3ffff) {
        return -1;
    }
    for(i = 0; i < PROCESSES; i++) {
        if(!bitIsSet(i)) {
            return i;
        }
    }
    return -1;
}

void launchNewProc() {
    //set new launch values;
    shm_data->launchSec = rand() % maxTimeBetweenNewProcsSecs + 1;
    shm_data->launchNano = rand() % maxTimeBetweenNewProcsNS + 1;
    // printf("launch %i.%i\n", shm_data->launchSec, shm_data->launchNano);

    // add to  to get new time to run
    shm_data->launchSec += osclock.seconds();
    shm_data->launchNano += osclock.nanoseconds();
    // printf("new launch %i.%i\n", shm_data->launchSec, shm_data->launchNano);
}



void ossClock() {
    // set up initial clock values operated by oss
    osclock.set(0, 0);
    printf("ossClock: clockInit %i:%i\n", osclock.seconds(), osclock.nanoseconds());
}

void updateClock(int sec, int nano) {

    if (nano >= 1000000000) {
        osclock.add(sec + nano / 1000000000, nano % 1000000000);
    } else {
        osclock.add(sec, nano);
    }
    //printf("updateClock: %i:%i\n", osclock.seconds(), osclock.nanoseconds());
}

void deinitSharedMemory() {
    if (shmdt(shm_data) == -1) {
        snprintf(perror_buf, sizeof(perror_buf), "%s: shmdt: ", perror_arg0);
        perror(perror_buf);
    }
    shmctl(shm_id, IPC_RMID, NULL);
}

void setBit(int b) {
    g_bitVector |= (1 << b);
}

// test if int i is set in bv
bool bitIsSet(int b) {
    return (g_bitVector & (1 << b)) != 0;
}

void clearBit(int b) {
    g_bitVector &= ~(1 << b);
}

void doSigHandler(int sig) {
    if (sig == SIGTERM) {
        // kill child process - reconfig to work with current code
        kill(getpid(), SIGKILL); // resend to child
    }
}

void bail() {
    deinitSharedMemory();
    if (sem_unlink(SEM_NAME) < 0) {
      perror("sem_unlink(3) failed\n");
    }
    kill(0, SIGTERM);
    exit(0);
}

void sigHandler(const int sig) {
    sigset_t mask, oldmask;
    sigfillset(&mask);

    // block all signals
    sigprocmask(SIG_SETMASK, &mask, &oldmask);

    if (sig == SIGINT) {
        printf("oss[%d]: Ctrl-C received\n", getpid());
        bail();
    }
    else if (sig == SIGALRM) {
        printf("oss[%d]: Alarm raised\n", getpid());
        bail();
    }
    sigprocmask(SIG_SETMASK, &oldmask, NULL);
}

int initializeSig() {
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask); // ignore next signals

    if(sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction");
        return -1;
    }

    // alarm and Ctrl-C(SIGINT) have to be handled
    sa.sa_handler = sigHandler;
    if ((sigaction(SIGALRM, &sa, NULL) == -1) ||    (sigaction(SIGINT, &sa, NULL) == -1)) {
        perror("sigaction");
        return -1;
    }
    alarm(ALARM_TIME);
    return 0;
}


/************** keep for future use if needed ***********************/


// void scheduler() {
//     PCB * foo;
//
//     foo = createProcess();
//
//     // while(totalProcesses < 2) {
//     //
//     //     if (activeProcs < 2) {
//     //         int create = osclock.seconds() > shm_data->launchSec;
//     //
//     //         if(!create && osclock.seconds()) {
//     //             create = osclock.seconds() > shm_data->launchSec
// 		// 						&& osclock.nanoseconds() >= shm_data->launchNano;
//     //         }
//     //         if(create) {
//     //             // printf("current %0d:%09d\n", osclock.seconds(), osclock.nanoseconds());
//     //             // printf("lanuch  %0d:%09d\n", shm_data->launchSec, shm_data->launchNano);
//     //             foo = createProcess();
//     //             launchNewProc();
//     //         }
//     //     }
//     //     osclock.add(0,1000000);
//     //     //sleep(1);
//     // }
//     //printf("total processes = 40\n");
//     //bail();
// }

// void releaseResources(int id) {
//   //shm_data = shmAttach();
//   int i;
//   printf("id = %i\n", id);
//   printf("current available:\n");
//   printf("     ");
//   for (i = 0; i < RESOURCES; i++) {
//     printf("%02d ",shm_data->r_state.available[i]);
//   }
//   printf("\nalloc resources:\n");
//   printf("     ");
//   for (i = 0; i < RESOURCES; i++) {
//     printf("%02d ",shm_data->r_state.alloc[id][i]);
//   }
//   printf("\nwork resources:\n");
//   printf("     ");
//   for (i = 0; i < RESOURCES; i++) {
//     printf("%02d ",shm_data->r_state.work[id][i]);
//   }
//   printf("\nclaim resources:\n");
//   printf("     ");
//   for (i = 0; i < RESOURCES; i++) {
//     printf("%02d ",shm_data->r_state.claim[id][i]);
//   }
//   printf("\n");
//   for (i = 0; i < RESOURCES; i++) {
//     shm_data->r_state.available[i] += shm_data->r_state.alloc[id][i];
//     shm_data->r_state.work[id][i] += shm_data->r_state.alloc[id][i];
//     shm_data->r_state.alloc[id][i] = 0;
//   }
//
//   printf("\nnew available:\n");
//   printf("     ");
//   for (i = 0; i < RESOURCES; i++) {
//     printf("%02d ",shm_data->r_state.available[i]);
//   }
//   // printf("\nnew current :\n");
//   // printf("     ");
//   // for (i = 0; i < RESOURCES; i++) {
//   //   printf("%02d ",shm_data->ptab.pcb[id].rsrcsNeeded[i]);
//   // }
//   printf("\nnew alloc:\n");
//   printf("     ");
//   for (i = 0; i < RESOURCES; i++) {
//     printf("%02d ",shm_data->r_state.alloc[id][i]);
//   }
//   printf("\nnew work:\n");
//   printf("     ");
//   for (i = 0; i < RESOURCES; i++) {
//     printf("%02d ",shm_data->r_state.work[id][i]);
//   }
//   printf("\n new claim resources:\n");
//   printf("     ");
//   for (i = 0; i < RESOURCES; i++) {
//     printf("%02d ",shm_data->r_state.claim[id][i]);
//   }
//
// }
