#ifndef Kernel_TASKS_H_
#define Kernel_TASKS_H_
/* Task include file. */
#include "platform.h"
#include "homeOS.h"

#define MAX_PRIORITY 255
#define NUMOFPRIORITIES 256
#define READY 1
#define BLOCKED 2
/* Main scheduler code */
extern TASK taskList[NUMOFPRIORITIES];
extern TASK currentTask;
extern TASK blockedList[NUMOFPRIORITIES];
extern void idleLoop(void *arg);
static void listAddBefore(TASK *currentPtr, TASK newPtr);
static void listRemove(TASK *list, TASK entry);
#endif
