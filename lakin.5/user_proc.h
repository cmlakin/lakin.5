#pragma once

#include "config.h"
#include "shm.h"
#include "oss.h"
#include "osclock.h"
#include "queue.h"

static int id;
sem_t *semaphore;


void uprocInitialize();
void attachSharedMemory();
void requestResources();
void releaseResources();
