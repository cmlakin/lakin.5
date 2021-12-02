#include <stdbool.h>

#include "config.h"
#include "queue.h"


Queue all_queues[RESOURCES];

int queueShift(Queue * queue);
void queuePush(Queue * queue, int processId);

queueItem * newItem(int processId) {
    queueItem * new = (queueItem *)malloc(sizeof(queueItem));

    new->next = NULL;
    new->processId = processId;
    return new;
}

void enqueue(int index, int processId) {
    queuePush(&all_queues[index], processId);
}

int dequeue(int index) {
    return queueShift(&all_queues[index]);
}

void queuePush(Queue * queue, int processId) {
    queueItem  * new = newItem(processId);


    // printf("push head %x tail %lx\n", (int)q->head, (long)q->tail);

    if(queue->tail == NULL) {
        queue->tail = new;
        queue->head = queue->tail;
    } else {
        queue->tail->next = new;
        queue->tail = new;
    }
    return;
}

int queueShift(Queue * queue) {
    queueItem * item = queue->head;

    if(item == NULL) {
        queue->tail = NULL;
        return -1;
    }
    queue->head = queue->head->next;
    if(queue->head == NULL) {
        queue->tail = NULL;
    }
    return item->processId;
}

// void queueDump(int index, char * indent) {
//     priorityQueue * q = getQueue(index);
//
//     priorityItem *h = q->head;
//
//
//     while(h != NULL) {
//         h = h->next;
//     }
//
// }

// void enqueueHigh(PCB * pcb) {
//     queuePush(QT_HIGH_PRIORITY, pcb);
// }
//
// PCB *dequeueHigh() {
//     return queueShift(QT_HIGH_PRIORITY);
// }
//
//
// void enqueueBlocking(PCB * pcb, int ossec, int ossnano);
// PCB * dequeueBlocking(int ossec, int ossnano);
//
// void createQueues() {
//     queues.lowPriority.enqueue = enqueueLow;
//     queues.lowPriority.dequeue = dequeueLow;
//     queues.highPriority.enqueue = enqueueHigh;
//     queues.highPriority.dequeue = dequeueHigh;
//     queues.blocking.head = NULL;
//     queues.blocking.enqueue = enqueueBlocking;
//     queues.blocking.dequeue = dequeueBlocking;
// }
//


// void enqueueBlocking(PCB * pcb, int ossec, int ossnano) {
//     // generate r.s seconds where r and s are random numbers with range
//     // r <= {0...5} and s <= {0...1000}
//     blockingItem * current = queues.blocking.head ;
//     blockingItem * previous = queues.blocking.head ;
//     blockingItem * new = (blockingItem *)malloc(sizeof(blockingItem));
//
//     new->next = NULL;
//     new->dqtime.sec = ossec + (rand() % 6);
//     new->dqtime.nano = ossnano + (rand() % 1001 * 1000 * 1000);
//     new->pcb = pcb;
//     //
//     // save these in pcb for testing
//     //
//     // pcb->testsec = new->dqtime.sec;
//     // pcb->testnano = new->dqtime.nano;
//
//     if(queues.blocking.head  == NULL) {
//         queues.blocking.head  = new;
//         return;
//     }
//
//     while(current != NULL && new->dqtime.sec >= current->dqtime.sec && new->dqtime.sec >= current->dqtime.nano) {
//         previous = current;
//         current = current->next;
//     }
//
//     if(current == NULL) {
//         previous->next = new;
//     } else if(current == queues.blocking.head ){
//         new->next = current;
//         queues.blocking.head  = new;
//     } else {
//         new->next = current;
//         previous->next = new;
//     }
// }
//
// PCB * dequeueBlocking(int ossec, int ossnano) {
//     if(queues.blocking.head == NULL) {
//         return NULL;
//     }
//     PCB * dequeued = NULL;
//     int qsec = queues.blocking.head->dqtime.sec;
//     int qnano = queues.blocking.head->dqtime.nano;
//
//
//     if(qsec > ossec) {
//         return NULL;
//     }
//
//     if(qsec == ossec && qnano > ossnano) {
//         return NULL;
//     }
//     //
//     // here
//     //  if qsec < ossec we don't care about nanoseconds so we're going to dequeue
//     //  otherwise qsec == ossec and qnano <= osssnano so we're going to dequeue
//     //
//     dequeued = queues.blocking.head->pcb;
//     queues.blocking.head = queues.blocking.head->next;
//     //
//     // TODO free malloced memory
//     //
//
//     return dequeued;
// }
