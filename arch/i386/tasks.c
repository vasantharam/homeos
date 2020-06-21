#include "homeOStypes.h"
#include "errorcodes.h"
#include "utilities.h"
#include "tasks.h"
#include "homeOS.h"
#include "io.h"
#include "i386utilities.h"
#include "i386arch.h"
#include "i386interrupts.h"
void archInit()
{
    int32 loop;
    uint32 staticGDTEntries[] =
    {
        0x0, 0x0, /* dummy */
        0x0, 0x0, /* unused */
        0xffff, 0x00CF9A00, /* kernel code segment 4G */
        0xffff, 0x00CF9200, /* kernel data segment 4G */

        0xffff, 0x00CFFA00, /* user code segment 4G */
        0xffff, 0x00CFF200, /* user data segment 4G */

        0x90000080, 0x00108900, /* TSS entry */
                                /* Hard coded to 0x9000 - 0x9080! */
        0x230000, 0x0000ec00, /* Call-gate for user jump */
                                /* Hard coded with user-code seg-sel RPL=3 */

    };
    GDT *gdt = (GDT *)GDT_ADDR;
    GDTR *gdtr = (GDTR *)GDTR_ADDR;
    TSS *tss = (TSS *)TSS_ADDR;
    gdtr->base = GDT_ADDR;
    gdtr->limit = GDT_SIZE;
    for (loop = 0; loop<128; loop++)
        ((uint8 *)tss)[loop] = 0;
    for (loop = 0; loop < 16; loop++)
    {
        *((uint32 *)gdt + loop) = staticGDTEntries[loop];
    }

    archInterruptInit();
    __asm__ (
        "lgdt %1\n\t"
        "mov %%esp, %0\n\t"
        "mov $0x30, %%ax\n\t"
        "ltr %%ax\n\t"
        :"=m"(tss->ESP)
        :"m"(*gdtr)
    );

    archConsoleInit();
}

/* Created tasks return to this function start. */
void archReturnFromTask(TASK task, void *arg)
{
	printk("return from task\n");
	/* What to do about stack, if its the last task,
	 * idleLoop should get picked, up, idle loop's stack?? 
	 * scheduler Algorithm i.e scheduleNewTask, should take
	 * care of this properly. 
	 */
#ifdef DEBUGPRINT
	printk("Task ID @return From Task: %d", (int)(task));
	printk("arg @return From Task: %x", (int)(arg));
#endif
	deleteTask(task);
	/* We should not get here, there is no other return
	 * address on the task, we will go nowhere, so better to
	 * wait here all day*/
#ifdef DEBUGPRINT
	printk("Ideally, this code, should not hit, we cant get out of the idle loop to get here\n");
#endif
	while(1) printk("Inf");
}

#define ASSERT \
    __asm__("mov $0, %%eax\n\t"\
            "mov %%eax, %%cs\n\t"\
    :);
static void archGenericEntryPoint(void (*fn)(void *arg), void *arg, void *stack, int stackSize)
{
    TSS *tss = (TSS *)TSS_ADDR;
    register int kernelESP = (int)((char *)stack-44);
    register int rstack = (int)stack;
    printk("archGen called with fn=%x arg=%x stack=%x stacksize=%x", (int)fn, (int)arg, (int)stack, (int)stackSize);
    /* Its bad to have kernel stack just at the bottom of
    user stack. This is temporary, needs fixing. 
    Once memory manager is in place, allocate kernel stack separately
    */
    /*stack-4 is left for higher layer to put something on */
    //printk("fn=%x arg=%x, stack=%x stacksize=%x\n", (int)fn, (int)arg, (int)stack, stackSize);
    tss->ESP0 = (int)((char *)stack-4); /* Re-entry stack */
    tss->SS0 = 0x18;
	*((int *)((char *)rstack-28)) = (int)0x2b; 
	*((int *)((char *)rstack-32)) = (int)((char *)rstack-500); 
	*((int *)((char *)rstack-36)) = (int)arg; 
	*((int *)((char *)rstack-40)) = (int)0x23; 
	*((int *)((char *)rstack-44)) = (int)fn; 
    __asm__(
        "mov %0, %%ebx\n\t"
        "mov $0x2b, %%eax\n\t"
        "mov %%eax, %%ds\n\t"
        "mov %%eax, %%es\n\t"
        "mov %%eax, %%gs\n\t"
        "mov %%eax, %%fs\n\t"
        "mov %%ebx, %%esp\n\t"
        "sti\n\t"
        "lret $4\n\t" /* I cant imagine its in bytes */
        :
        :"r"(kernelESP)
    );
     
}

void archCreateTask(ArchTaskStruct *archTaskStruct, void (*fn)(void *arg), void *arg, void *stack, int stackSize)
{
	/* When a task is started, the task stack is pushed fist with
	 * the task id for later use, then the argument, then the
	 * return address. */
    printk("stack(archCreateTask):%x\n", stack);
    archTaskStruct->ESP = (int)((char *)stack-24);
    archTaskStruct->ESP0 = (int)((char *)stack-4); /* Re-entry stack */
#ifdef DEBUGPRINT
	printk("arg: %d\n", (int)arg);
#endif
    /*
    *((int *)((char *)stack-4)) is used in higher layers to
    store task struct */
	*((int *)((char *)stack-8)) = (int)stackSize; 
	*((int *)((char *)stack-12)) = (int)stack; 
	*((int *)((char *)stack-16)) = (int)arg;
	/* Task stack is pushed with the arg */
	*((int *)((char *)stack-20)) = (int)fn;
	/* Task starts @ fn */
	archTaskStruct->EIP = (int)archGenericEntryPoint;
}

void archTaskDeleted(ArchTaskStruct *archTaskStruct)
{
	/* As of now can't think of anything innovative to do
	 * in this notification function call. */
}

int schedulerSpinLock;
/* Implement a spin lock here. */
void archLockScheduler(void)
{
    int ret;
#ifdef DEBUGPRINT
    printk("Lock: Disabling interrupts");
#endif
    ret = archAtomicIncrement32(&schedulerSpinLock);
    if (ret==1) return;
    else while(schedulerSpinLock);
}
void archUnlockScheduler(void)
{
#ifdef DEBUGPRINT
    printk("Unlock Enabling interrupts");
#endif
    schedulerSpinLock = 0;
}
/* Lets use disable interrupt as well, which is different
 * from the above procedure. This will block all hardware interrupts. */
void archDisableInterrupt(void)
{
#ifdef DEBUGPRINT
    printk("Disabling interrupts");
#endif
	__asm__("cli\n\t");
}
void archEnableInterrupt(void)
{
#ifdef DEBUGPRINT
    printk("Enabling interrupts");
#endif
	__asm__("sti\n\t");
}
/* The main task branching code. */
/* It might appear, its unessential to store, the EIP, but actually,
 * it is essential to do so. Because, during task creation, EIP stored,
 * will be different and not @ ret_here. Durint task creation it is 
 * the function start address. */
/* In case the RET happens for a new task, it would continue from the
 * task's begenning. */
void archSwitchTo(ArchTaskStruct *prevTask, ArchTaskStruct *nextTask)
{
	/* This is a critical code, the prevTask and nextTask
	 * variables are on the  stack of the current task, changing the
	 * stack means, getting not uptodate values of prevTask and
	 * nextTask. */
    TSS *tss = (TSS *) TSS_ADDR;
    printk("%x\n", prevTask->ESP);
    printk("%x\n", nextTask->ESP);
    tss->ESP0 = nextTask->ESP0; 
	__asm__ __volatile__(
			"movl $ret_here, %1\n\t"
			"movl %%esp, %0\n\t"
			"movl %%ebp, %2\n\t"
			"movl %5, %%ebp\n\t"
			"movl %3, %%esp\n\t" /* %x are invalid after this line. */
			"push %4\n\t" /* This is ok since, its on a register.*/
            "sti\n\t"
			"ret\n\t"
			"ret_here:\n\t"
			:"=m" (prevTask->ESP), "=m" (prevTask->EIP), "=m"(prevTask->EBP)
			:"r" (nextTask->ESP), "r" (nextTask->EIP), "r" (nextTask->EBP)
			);
#ifdef DEBUGPRINT
	printk("\nReturn from switch");	
#endif
}

void archGetTimeStamp(int *timeStamp)
{
}

void archIdleLoop(void *arg)
{
	while(1)
	{
		__asm__("hlt\n\t");
	}
}

int archAtomicIncrement32(int *sharedInteger)
{
    int temp = (int)sharedInteger;
    __asm__(
    "tryAgain: mov (%0), %%eax\n\t"
    "mov %%eax, %%ebx\n\t"
    "mov %%ebx, %%ecx\n\t"
    "inc %%ebx\n\t"
    "lock cmpxchg %%ebx ,(%0)\n\t"
    "cmp %%ecx, %%eax\n\t"
    "jnz tryAgain"
    :
    :"r"(temp)
    :"%eax", "%ebx", "%ecx"
    );
    return *sharedInteger;
}
