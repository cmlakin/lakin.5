#pragma once

#include "config.h"
#include "shm.h"
#include "oss.h"
#include "osclock.h"
#include "queue.h"

static int id;
PCB * pcb;

void uprocInitialize();
void attachSharedMemory();
void requestResources();
void releaseResources();
