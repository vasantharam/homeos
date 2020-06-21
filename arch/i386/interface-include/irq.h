#ifndef interface_i386irq__h__
#define interface_i386irq__h__
#define MAX_IRQS 32
typedef void ArchIRQHandler(void);

extern void archIRQInit(void);
extern void archEnableIRQ(int irq);
extern void archDisableIRQ(int irq);
extern int archRegisterIRQHandler(int irq, ArchIRQHandler *irqHandler);

#endif
