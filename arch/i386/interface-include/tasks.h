#ifndef i386tasks__h__
#define i386tasks__h__
/* Task include file. */
typedef struct __attribute__((packed))
{
	/* This is the TSS structure in i386 */
	/* In this implementation its not essential to have this
	 * struct same as the i386 TSS structure, it is maintained, so
	 * inorder to fascilatate the possibility of utilizing processors's
	 * TSS related features.*/
	int prevTask;
	int ESP0;
	int SS0;
	int ESP1;
	int SS1;
	int ESP2;
	int SS2;
	int CR3;
	int EIP;
	int EFLAGS;
	int EAX;
	int ECX;
	int EDX;
	int EBX;
	int ESP;
	int EBP;
	int ESI;
	int EDI;
	int ES;
	int CS;
	int DS;
	int FS;
	int GS;
	int LDTSS;
	int IOMBA;
} ArchTaskStruct;

extern void archInit(void);
extern int archAtomicIncrement32(int *sharedInteger);
extern void archCreateTask(ArchTaskStruct *archTaskStruct, void (*fn)(void *arg), void *arg, void *stack, int stackSize);
extern void archTaskDeleted(ArchTaskStruct *archTaskStruct);
extern void archLockScheduler(void);
extern void archUnlockScheduler(void);
extern void archDisableInterrupt(void);
extern void archEnableInterrupt(void);
extern void archSwitchTo(ArchTaskStruct * currentTask, ArchTaskStruct * nextTask);
extern void archGetTimeStamp(int *timeStamp);
extern void archIdleLoop(void *arg);
#endif
