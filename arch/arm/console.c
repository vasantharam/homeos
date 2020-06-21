#include "homeOS.h"
#include <stdarg.h>
#include <limits.h>
#include "console.h"
void consoleInit(void)
{
    int temp;
    volatile int *uartBase = (volatile int *)UARTBASE;
    /* Disable everything*/
//    *(volatile int *)(uartBase + UARTCR_OFFSET) = 0x0;
    /* No Break, No parity, No EPS, No STP2, FEN=1, WLEN=8, No SPS,  */
    /* Integer and Fractional Baud set such that 
     * Baud rate = 0x14400*/
    *(volatile int *)(uartBase + UARTCR_OFFSET) = 0x0;

    *(volatile int *)(uartBase + UARTIBRD_OFFSET) = 0xc;
    *(volatile int *)(uartBase + UARTFBRD_OFFSET) = 0x8;
    
    *(volatile int *)(uartBase + UARTLCR_H_OFFSET) = 0x70; 

    /* Enable the UART  */
    /* TXEn, RxEn, UARTEn*/
    *(volatile int *)(uartBase + UARTCR_OFFSET) = 0x0301;

    printk("Work please %d\n", temp);
}

int kputs(char *str)
{
    volatile int *uartBase = (volatile int *)UARTBASE;
    int result;
    while (*str)
    {
        /* Wait until there is space in transmit FIFO*/
        do
        {
            result = *(volatile int *)(uartBase + UARTFR_OFFSET);
            result &= 16;
        } while (result);
        *(volatile int *)(uartBase + UARTDR_OFFSET) = str[0];
        str++;
    }

	return OK;
}

#define isdigit(c)      ((unsigned) ((c) - '0') <  (unsigned) 10)
#define count_putc(c) do {char str[2]; charcount++; str[0] = c; str[1]=0; kputs(str);} while(0)
typedef int ptrdiff_t;     
int printk(char *fmt, ...)
{
        int c, charcount = 0;
        enum { LEFT, RIGHT } adjust;
        enum { LONG, INT } intsize;
        int fill;
        int width, max, len, base;
        static char X2C_tab[]= "0123456789ABCDEF";
        static char x2c_tab[]= "0123456789abcdef";
        char *x2c;
        char *p;
        long i;
        unsigned long u;
        char temp[8 * sizeof(long) / 3 + 2];

        va_list argp;
      

        va_start(argp, fmt);

        while ((c= *fmt++) != 0) {
                if (c != '%') {
                        /* Ordinary character. */
                        count_putc(c);
                        continue;
                }

                /* Format specifier of the form:
                 *      %[adjust][fill][width][.max]keys
                 */
                c= *fmt++;

                adjust= RIGHT;
                if (c == '-') {
                        adjust= LEFT;
                        c= *fmt++;
                }

                fill= ' ';
                if (c == '0') {
                        fill= '0';
                        c= *fmt++;
                }

                width= 0;
                if (c == '*') {
                        /* Width is specified as an argument, e.g. %*d. */
                        width= va_arg(argp, int);
                        c= *fmt++;
                } else
                if (isdigit(c)) {
                        /* A number tells the width, e.g. %10d. */
                        do {
                                width= width * 10 + (c - '0');
                        } while (isdigit(c= *fmt++));
                }

                max= INT_MAX;
                if (c == '.') {
                        /* Max field length coming up. */
                        if ((c= *fmt++) == '*') {
                                max= va_arg(argp, int);
                                c= *fmt++;
                        } else
                        if (isdigit(c)) {
                                max= 0;
                                do {
                                        max= max * 10 + (c - '0');
                                } while (isdigit(c= *fmt++));
                        }
                }

                /* Set a few flags to the default. */
                x2c= x2c_tab;
                i= 0;
                base= 10;
                intsize= INT;
                if (c == 'l' || c == 'L') {
                        /* "Long" key, e.g. %ld. */
                        intsize= LONG;
                        c= *fmt++;
                }
                if (c == 0) break;

                switch (c) {
                        /* Decimal. */
                case 'd':
                        i= intsize == LONG ? va_arg(argp, long)
                                                : va_arg(argp, int);
                        u= i < 0 ? -i : i;
                        goto int2ascii;

                        /* Octal. */
                case 'o':
                        base= 010;
                        goto getint;

                        /* Pointer, interpret as %X or %lX. */
                case 'p':
                        if (sizeof(char *) > sizeof(int)) intsize= LONG;

                        /* Hexadecimal.  %X prints upper case A-F, not %lx. */
                case 'X':
                        x2c= X2C_tab;
                case 'x':
                        base= 0x10;
                        goto getint;

                        /* Unsigned decimal. */
                case 'u':
                getint:
                        u= intsize == LONG ? va_arg(argp, unsigned long)
                                                : va_arg(argp, unsigned int);
                int2ascii:
                        p= temp + sizeof(temp)-1;
                        *p= 0;
                        do {
                                *--p= x2c[(ptrdiff_t) (u % base)];
                        } while ((u /= base) > 0);
                        goto string_length;

                        /* A character. */
                case 'c':
                        p= temp;
                        *p= va_arg(argp, int);
                        len= 1;
                        goto string_print;

                        /* Simply a percent. */
                case '%':
                        p= temp;
                        *p= '%';
                        len= 1;
                        goto string_print;

                        /* A string.  The other cases will join in here. */
                case 's':
                        p= va_arg(argp, char *);

                string_length:
                        for (len= 0; p[len] != 0 && len < max; len++) {}

                string_print:
                        width -= len;
                        if (i < 0) width--;
                        if (fill == '0' && i < 0) count_putc('-');
                        if (adjust == RIGHT) {
                                while (width > 0) { count_putc(fill); width--; }
                        }
                        if (fill == ' ' && i < 0) count_putc('-');
                        while (len > 0) { count_putc((unsigned char) *p++); len--; }
                        while (width > 0) { count_putc(fill); width--; }
                        break;

                        /* Unrecognized format key, echo it back. */
                default:
                        count_putc('%');
                        count_putc(c);
                }
        }

        /* Mark the end with a null (should be something else, like -1). */
        va_end(argp);
        return charcount;
}

