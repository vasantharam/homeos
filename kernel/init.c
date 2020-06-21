#include "homeOStypes.h"
#include "platform.h"
#include "errorcodes.h"
#include "homeOS.h"
extern void idleLoop(void *arg);

struct TaskStruct idleTaskStruct;
TASK idleTaskID = &idleTaskStruct;
char idleSTACK[1024];
int32 kernelInit(void)
{
    archInit();
    schedulerInit();

    idleTaskStruct.priority = 0;
    createTask(&idleTaskID, idleLoop, NULL, (void *)(idleSTACK + 1024), 1024);
    return OK;

}
