#include "config.h"
#include "shm.h"


char perror_buf[50];
const char * perror_arg0 = "uprocess";
//static char * FTOK_BASE[PATH_MAX];

static int shm_id;
static int msg_id;
static int foo;

void uprocInitialize();
void doit();
void attachSharedMemory();

char strbuf[20];


int main (int argc, char ** argv){
    int id = atoi(argv[1]);

    foo = id;

    srand(getpid());


    uprocInitialize();
    attachSharedMemory();
    doit(id);
}

void doit(int id) {
    while(1) {
        ipcmsg msg;

        msg.mtype = msg.ossid;

        // strcpy(msg.mtext, strbuf);
        // snprintf(&msg.mtext[0],sizeof(msg.mtext), "from %ld",  id);
        if (msgsnd(msg_id, (void *)&msg, sizeof(msg), 0) == -1) {
            printf("oss msg not sent");
        }
        id = foo;

				if(msgrcv(msg_id, (void *)&msg, sizeof(ipcmsg), id + 1, 0) == -1) {
            printf("error receving message\n");
            exit(-1);
        }
    }
}

void uprocInitialize(){
    key_t sndkey = ftok(FTOK_BASE, FTOK_MSG);

    if (sndkey == -1) {

        snprintf(perror_buf, sizeof(perror_buf), "%s: ftok: ", perror_arg0);
        perror(perror_buf);
    }

    msg_id=msgget(sndkey, 0666 );
}

void attachSharedMemory() {
    key_t fkey = ftok(FTOK_BASE, FTOK_SHM);

    shm_id = shmget(fkey, sizeof(struct shared_data), 0666 | IPC_CREAT);

    if(shm_id == -1) {
        snprintf(perror_buf, sizeof(perror_buf), "%s: shmget: ", perror_arg0);
        perror(perror_buf);
        return;
    }

    shm_data = (struct shared_data*)shmat(shm_id, NULL, 0);
}
