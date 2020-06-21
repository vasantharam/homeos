#ifndef i386internal__interrupts__h__
#define i386internal__interrupts__h__
#include "homeOStypes.h"
extern uint32 archInterruptInit(void);
extern void intr0(void);
extern void intr1(void);
extern void intr2(void);
extern void intr3(void);
extern void intr4(void);
extern void intr5(void);
extern void intr6(void);
extern void intr7(void);
extern void intr8(void);
extern void intr9(void);
extern void intr10(void);
extern void intr11(void);
extern void intr12(void);
extern void intr13(void);
extern void intr14(void);
extern void intr15(void);
extern void timerInterrupt(void);
extern void keyboardInterrupt(void);
typedef void (*InterruptHandler)(void);

typedef struct __attribute__((packed)) _IDT
{
    uint16 offset_lo;
    uint16 selector;
    uint8 always0;
    uint8 flags;
    uint16 offset_hi;
} IDT;
#endif
