#ifndef arm__console__h__
#define arm__console__h__

#define UARTBASE 0x101f1000
#define UARTDR_OFFSET 0
#define UARTFR_OFFSET 6
#define UARTIBRD_OFFSET 9
#define UARTFBRD_OFFSET 10
#define UARTLCR_H_OFFSET 11
#define UARTCR_OFFSET 12
#define UARTIMSC_OFFSET 14
#define UARTIFLS_OFFSET (0x34/4)

extern void consoleInit(void);
#endif
