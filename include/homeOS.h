#ifndef __TASKS__H__
#define __TASKS__H__
/* Task include file. */
#include "platform.h"
/* TODO: Do not expose TaskStruct to application. */
struct TaskStruct
{
	int state; // running or blocked
	struct TaskStruct *next, *prev; // circularly linked list
	int priority;
	int reselectTaskList;
	int blockedTimeStamp;
	int blockedTimeout;
	ArchTaskStruct archTaskStruct;
};
/* Main scheduler code */
typedef struct TaskStruct *TASK ;
int createTask(TASK *task, void (*fn)(void *arg), void *arg, void *stack, int stackSize);
int deleteTask(TASK taskID);
int blockTask(int timeout);
int unblockTask(TASK taskID);
void schedulerInit(void);
void scheduleNewTask(void);
/* TODO: schedulerInit should also do schedulerStart, why additional entity. */
void schedulerStart(void);

int32 kernelInit(void);
#endif
