/* Main scheduler code */
#include "platform.h"
#include "ktasks.h"
#include "errorcodes.h"
#include "utilities.h"
/* This is the ready tasks list*/
TASK taskList[NUMOFPRIORITIES];
/* Having a seperate blocked task list can speed up 
 * traversal.
 */
TASK blockedList[NUMOFPRIORITIES];
TASK currentTask = NULL;

void schedulerInit(void)
{
	int i;
	currentTask = NULL;
#ifdef DEBUGPRINT
	printk("currentTask filled to NULL: %d\n", (int)currentTask);
#endif
	for (i=0; i<NUMOFPRIORITIES; i++)
	{
		blockedList[i] = NULL;
		taskList[i] = NULL;
	}
}

void schedulerStart (void)
{
    archIRQInit();
    archEnableInterrupt();
	while (1) {printk("SchedulerStart\n"); scheduleNewTask();}
}
void scheduleNewTask(void)
{
	int priority = 256;
	TASK prevTask, nextTask;
#ifdef DEBUGPRINT
	printk("currentTask in scheduleNewTask very start:%d\n", (int)currentTask);
#endif

	if (!currentTask->reselectTaskList)
	{
#ifdef DEBUGPRINT
		printk("\nSame TaskList");
#endif
		nextTask = currentTask->next;
		/* if I have a seperate list for ready and blocked tasks,
		 * then the problem of traversing until finding a free one
		 * is removed, making it faster. Point taken and code changed.
		 * */
	}
	else 
	{
#ifdef DEBUGPRINT
		printk("\nReselect TaskList");
#endif
		currentTask->reselectTaskList = FALSE;
		while (!taskList[--priority]);
		nextTask = taskList[priority]->next;
	}
#ifdef DEBUGPRINT
	printk("currentTask in scheduleNewTask:%d\n", (int)currentTask);
#endif
	prevTask = currentTask;
	currentTask = nextTask;
	taskList[currentTask->priority] = currentTask;
#ifdef DEBUGPRINT
		printk("\nprevTask: %d\n", (int)prevTask);
		printk("\ncurrentTask/nextTask: %d\n", (int)currentTask);
#endif
	if ((nextTask!=prevTask) && (nextTask->priority == 0))
	{
#ifdef DEBUGPRINT
		printk("idleLoop start");
		idleLoop(NULL);
#endif
	}
	else if ((nextTask != prevTask))
	{
#ifdef DEBUGPRINT
		printk("before archSwitchTo");
#endif
		archSwitchTo(&(prevTask->archTaskStruct), &(nextTask->archTaskStruct));
#ifdef DEBUGPRINT
		printk("After archSwitchTo");
#endif
	}
}
int createTask(TASK *task, void (*fn)(void *arg), void *arg, void *stack, int stackSize)
{
	/* In the present implementation memory should be 
	 * provided by the user of createTask, this help the
	 * small foot print philosophy.
	 * */
	int priority = (*task)->priority;
	(*task)->state = READY;
    
#ifdef DEBUGPRINT
	printk("currentTask: %d\n", (int)currentTask);
#endif

    if (!currentTask) 
	{
#ifdef DEBUGPRINT
		printk("CreateTask: !currentTask true, so making this task the current Task\n");
#endif
		currentTask = *task;
	}
	if ((priority > currentTask->priority))
		currentTask->reselectTaskList = TRUE;
	/* archTaskCreate should be above listAdd, to prevent,
	 * the task from kicking up before archTaskCreate. */
    *((int *)((char *)stack -4)) = (int) (*task);
	archCreateTask(&((*task)->archTaskStruct), fn, NULL, stack, stackSize);
	listAddBefore(&(taskList[priority]), *task);
	return OK;

}
int deleteTask(TASK taskID)
{
	/* No need for disable interrupt, if task gets scheduled
	 * before remove, no harm, lets delete after this time slice. 
	 * But what happens if the switch happens after the ready check,
	 * and that task gets blocked, its not already in the ready list
	 * making deletion not work. So any way lets disable interrupt
	 * for now. */
	archLockScheduler();
#ifdef DEBUGPRINT
	printk("Remove from appropriate list");
#endif
	if (taskID->state == READY)
	{
		listRemove(&(taskList[taskID->priority]), taskID);
		if (taskList[taskID->priority] == NULL) currentTask->reselectTaskList = TRUE;
	}
	else 
	{
		listRemove(&(blockedList[taskID->priority]), taskID);
	}
	archUnlockScheduler();
	archTaskDeleted(&(taskID->archTaskStruct));
#ifdef DEBUGPRINT
	printk("Just before start of scheduler , in deleteTask\nNote: There should be a taskIDmath printed after this:\n");
#endif
	if (currentTask == taskID) 
	{
#ifdef DEBUGPRINT
		printk("Task ID match");
#endif
		scheduleNewTask();
	}
	return OK;
}
int blockTask(int timeout)
{
	/* Only currentTask can call block on itself,
	 * this prevents idleTask from accedently becoming blocked.*/
	/* TODO: Remove from ready queue and add to blocked queue. */
	/* If a task switch happens just after remove from queue,
	 * before add to blocked queue, the task can be lost in the air,
	 * so the order has to be add to blocked list , then
	 * remove from ready list. */
	int priority = currentTask->priority;
	int timeStamp;
	if (currentTask->next == currentTask) 
		currentTask->reselectTaskList = TRUE;
	currentTask->state = BLOCKED;
	listAddBefore(&(blockedList[priority]), currentTask);
	listRemove(&(taskList[priority]), currentTask);
	archGetTimeStamp(&currentTask->blockedTimeStamp);
	scheduleNewTask();
	/* Here we get if only unblocked */
	archGetTimeStamp(&timeStamp);
	if (currentTask->blockedTimeout <= (timeStamp-currentTask->blockedTimeStamp ))
	{
		return ERR_SEM_TIMEOUT;
	}
	return OK;

}
int unblockTask(TASK taskID)
{
	int priority = taskID->priority;
	taskID->state = READY;
	listRemove(&(blockedList[priority]), taskID);
	listAddBefore(&(taskList[priority]), taskID);
	if (priority > currentTask->priority) 
		currentTask->reselectTaskList = TRUE;
	return OK;
}

void idleLoop(void *arg)
{
//	printk("Idle loop has started\n");
	while (1)
	{
//		printk("idleLoop");
		archIdleLoop(arg);
	}
}

static void listAddBefore(TASK *currentPtr, TASK newPtr)
{
	/* circularly linked list. */
	if (*currentPtr == NULL) 
	{
		*currentPtr = newPtr;
		newPtr->prev = newPtr;
		newPtr->next = newPtr;
		return;
	}
	newPtr->prev = (*currentPtr)->prev;
	newPtr->next = *currentPtr;
	(*currentPtr)->prev->next = newPtr;
	(*currentPtr)->prev = newPtr;
}

static void listRemove(TASK *list, TASK entry)
{
	TASK cur = (*list)->next;
	while (cur!=entry && cur!=(*list))
	{
#ifdef DEBUGPRINT
		printk("\nlistRemove cur: %d\n", (int)cur);
#endif
		cur = cur->next;
	}
	if (cur!=entry) return;
	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	if (*list == cur)
		*list = cur->prev;
	if (*list == cur)
		*list = NULL;
	
}
