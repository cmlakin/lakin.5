#pragma once

struct state * initializeResources();
struct state * claimMatrix(PCB *, int);
struct state * allocMatrix();
struct state * workMatrix();
void printClaimMatrix();
void printAllocMatrix();
void printWorkMatrix();
