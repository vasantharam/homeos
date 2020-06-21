/* The main kernel starts here, for the time being.
 */
#include "cmain.h"
#include "homeOS.h"
#include "utilities.h"
void task1(void *arg);
void task2(void *arg);
/* This is an application and used to demonstrate the scheduler is working.
The code here is in a pathetic state, since it has repeatedly been patched
with more and more and more coee, with out any planning, it has for example,
lots of unused variables, like the APIC ones, a commented out IDE controller
test code etc.., it requires to be cleaned up soon. */
int main()
{
    int i;
	struct TaskStruct task1Struct;
	struct TaskStruct task2Struct;
    
	TASK task1ID = &task1Struct;
	TASK task2ID = &task2Struct;

	//char *apicTimer = 0xFEE00320;
	//char *apicInitialCount = 0xFEE00380;
	//char *apicCurrentCount = 0xFEE00390;
	//char *apicDivideConfig = 0xFEE003E0;
	task1Struct.priority = 1;
	task2Struct.priority = 1;

	char task1STACK[1024];
	char task2STACK[1024];
	char string[2];
	string[1] = 0;
	printk("==============================================================\n");
	printk("        WELCOME TO THE WORLD OF OPERATING SYSTEMS\n");
	printk("==============================================================\n");
    kernelInit();



	createTask(&task1ID, task1, NULL, (void *)(task1STACK+1024), 1024 );
	createTask(&task2ID, task2, NULL, (void *)(task2STACK+1024), 1024 );

#ifdef DEBUGPRINT
	printk("before irq_init\n");
#endif
	
//	*(int *)apicDivideConfig = 0xb; /* Divide-by 1. */
//	*(int *)apicTimer = 0x00020020;/* Periodic, not-masked, vector=0x20*/
//	*(int *)apicInitialCount = 0xf00;
#ifdef DEBUGPRINT
	printk("Before schedulerStart\n");
#endif
	schedulerStart();
#ifdef DEBUGPRINT
	printk("after irq_init\n");
#endif
	/* We should never get here, if we do so, it can be
	 * by some miracle. */
	printk("Something miraculous has happend, out of a while(1),in idle task...\n");
a:
	i=0;	
	while(i++<3270) printk("cmInf");
	goto a;
}
void interrupt_interface(char *msg)
{
//    printk(msg);
}
/* Hexa */

void task1(void *arg)
{
    int i;
	while(1) 
	{
        if ((i % 100000) == 0)
        printk(" task1 printed every 100000th time: %d \n", i);
        i++;
	}; 
}
void task2(void *arg)
{
    int i;
	while(1) 
	{
        if ((i % 444444) == 0)
        printk(" task2 printed every 444444th time : %d \n", i);
        i++;
	};
}
