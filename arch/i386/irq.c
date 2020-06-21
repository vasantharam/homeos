/* Lets get the timer interrupt 
 * enabled for the scheduler. */
#include "homeOStypes.h"
#include "errorcodes.h"
#include "utilities.h"
#include "i386timer.h"
#include "i386irq.h"
#include "io.h"
#include "irq.h"

unsigned irqMask=0xfffb;
#define mask21 (((unsigned char *)&irqMask)[0])
#define maskA1 (((unsigned char *)&irqMask)[1])

ArchIRQHandler *handler[MAX_IRQS] =
{ 
  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL,  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL,  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL,  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL, 
  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL,  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL,  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL,  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL, 
  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL,  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL,  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL,  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL, 
  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL,  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL,  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL,  (ArchIRQHandler *)NULL, (ArchIRQHandler *)NULL, 
};
void archIRQInit(void)
{
	int i;
	/* Set the timer interrupt occurance time
	 * to 100ms.  This is our scheduler time slice time approx.
	 * */
#ifdef DEBUGPRINT
	printk("Timer count set LSB of count: %x\n", (LATCH & 0xff));
	printk("MSB of count: %x\n", (LATCH>>8));
#endif
	inb(0x61, i);
	i= (i&~0x02) | 0x01;
	outb(i, 0x61);
	outb(0xb0, 0x43);
	outb((LATCH & 0xff), 0x42);
	outb((LATCH>>8), 0x42);


//	outb(0x34, 0x43);
//	outb(0xff, 0x40);
//	outb(0xff, 0x40);
	/* Enable irq zero, the timer interrupt */
	archEnableIRQ(0);
	archEnableIRQ(1);

}

void archEnableIRQ(int irq)
{
	unsigned mask = ~(1<<irq);
	irqMask &= mask;
	if (irq & 8)
	{
		outb(maskA1, 0xA1);
	}
	else
	{
		outb(mask21, 0x21);
	}
}
void archDisableIRQ(int irq)
{
	unsigned mask = (1<<irq);
	irqMask |= mask;
	if (irq & 8)
	{
		outb(maskA1, 0xA1);
	}
	else
	{
		outb(mask21, 0x21);
	}
}	

void doIRQ(int irq)
{
	int val;
    if (handler[irq] != (ArchIRQHandler *)NULL) handler[irq]();
	irqMask |= (1<<irq);
	if (irq&8)
	{
		inb(0xA1, val);
		outb(maskA1, 0xA1);
		outb(0x62, 0x20);
		outb(0x20, 0xA0);
	}
	else
	{
		inb(0x21, val);
		outb(mask21, 0x21);
		outb(0x20, 0x20);
	}
	outb(0xb0, 0x43);
	outb((LATCH & 0xff), 0x42);
	outb((LATCH>>8), 0x42);
	archEnableIRQ(irq);
#ifdef DEBUGPRINT
	printk("\ndo IRQ finished mask=%x irq=%d" , irqMask, irq);
#endif
}

int archRegisterIRQHandler(int irq, ArchIRQHandler *irqHandler)
{
/* TODO: implement IRQ call back mechanism */
    handler[irq] = irqHandler;
    return OK;
}
