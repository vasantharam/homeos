#include "errorcodes.h"
#include "homeOStypes.h"
#include "interrupts.h"
#include "i386interrupts.h"
#include "i386arch.h"
uint32 archInterruptInit(void)
{
    int32 i;
    InterruptHandler interruptAddr[16];
    IDTR *idtr = (IDTR *) IDTR_ADDR;
    IDT *idt=(IDT *) IDT_ADDR;
    idtr->base = IDT_ADDR;
    idtr->limit = IDT_SIZE;
	interruptAddr[0]=&intr0;
	interruptAddr[1]=&intr1;
	interruptAddr[2]=&intr2;
	interruptAddr[3]=&intr3;
	interruptAddr[4]=&intr4;
	interruptAddr[5]=&intr5;
	interruptAddr[6]=&intr6;
	interruptAddr[7]=&intr7;
	interruptAddr[8]=&intr8;
	interruptAddr[9]=&intr9;
	interruptAddr[0xa]=&intr10;
	interruptAddr[0xb]=&intr11;
	interruptAddr[0xc]=&intr12;
	interruptAddr[0xd]=&intr13;
	interruptAddr[0xe]=&intr14;
	interruptAddr[0xf]=&intr15;
    

    /* Initialize IDTs */
    for (i=0; i<=0xf; i++)
    {
        idt[i].offset_lo = 0xffff & (int32)(interruptAddr[i]);
        idt[i].offset_hi = (0xffff0000 & (int32)(interruptAddr[i])) >> 16;
        idt[i].selector = 0x10;
        idt[i].always0 = 0;
        idt[i].flags = 0x8e;
    }
    
    for (i=0x10; i<256; i++)
    {
        idt[i].offset_lo = 0xffff & (int32)(interruptAddr[0]);
        idt[i].offset_hi = (0xffff0000 & (int32)(interruptAddr[0])) >> 16;
        idt[i].selector = 0x10;
        idt[i].always0 = 0;
        idt[i].flags = 0x8e;
    }

    idt[0x20].offset_lo = 0xffff & (int32)(&timerInterrupt);
    idt[0x20].offset_hi = (0xffff0000 & (int32)(&timerInterrupt)) >> 16;


    idt[0x21].offset_lo = 0xffff & (int32)(&keyboardInterrupt);
    idt[0x21].offset_hi = (0xffff0000 & (int32)(&keyboardInterrupt)) >> 16;
    __asm__ volatile (
        "lidt   %0\n\t"
        :
        :"m"(*idtr)
    );
    return OK;
}
