#ifndef __i386interface_io_h
#define __i386interface_io_h

#define inb( port, val)\
	__asm__ __volatile__(\
			"inb %1, %%al\n\t"\
			:"=a"(val)\
			:"n"(port)\
			);
#define outb(val,  port)\
{\
    int temp=val;\
	__asm__ __volatile__(\
			"movl %0, %%eax\n\t"\
			"andl $0xFF, %%eax\n\t"\
			"outb %%al, %1\n"\
			:\
			:"g"(temp), "n"(port)\
			);\
}
#endif
