#ifndef __I386__ARCH__
#define __I386__ARCH__

#define GDTR_ADDR 0x8000
#define IDTR_ADDR 0x8008

#define GDT_ADDR 0x8100
#define GDT_SIZE 80
#define IDT_ADDR 0x8800
#define IDT_SIZE 2048 /* 256-vecs * 8-per-vec */

#define TSS_ADDR 0x9000
#define TSS_SIZE 128

#define KERNEL_CODE 2
#define KERNEL_DATA 3
#define USER_CODE 4
#define USER_DATA 5
#define TSS_SEL 6
#define CALLGATE 7

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
} TSS;

typedef struct __attribute__((packed)) _GIDTR
{
    uint16 limit;
    uint32 base;
} GDTR, IDTR;

#define EXTRACT(arg, field) (((arg) & (field##_MASK)) >> (field##_SHIFT))
#define GDT_OFFSETLOW_MASK 0xffff
#define GDT_OFFSETLOW_SHIFT 0
#define GDT_OFFSETMID_MASK 0xff0000
#define GDT_OFFSETMID_SHIFT 16
#define GDT_OFFSETHIGH_MASK 0xFF000000
#define GDT_OFFSETHIGH_SHIFT 24
typedef struct __attribute__((packed)) _GDT
{
    union
    {
        uint16 limit;
        uint16 offsetLow;
    };
    union
    {
        uint16 baseLow;
        uint16 selector;
    };
    uint8 baseMid; 
    union
    {
        uint16 flags; 
        struct 
        {
            uint8 unused;
            uint8 offsetMid;
        };
    };
    union
    {
        uint8 baseHigh;
        uint8 offsetHigh;
    };
} GDT;

#endif
