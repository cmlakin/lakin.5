#pragma once

struct state * initializeResources();
struct state * claimMatrix(PCB *, int);
struct state * allocMatrix();
void printClaimMatrix();
void printAllocMatrix();
