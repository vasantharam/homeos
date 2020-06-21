#include "homeOStypes.h"
#include "errorcodes.h"
#include "i386utilities.h"
#include "i386scancodes.h"
#include "i386irq.h"
#include "utilities.h"
#include "tasks.h"
#include "homeOS.h"

int32 lowKeys[MAX_KEYCODES] =
{
LOWKEYS
};

int32 highKeys[MAX_KEYCODES]=
{
HIGHKEYS
};

static int processKeys(uint32 data)
{
    static int32 shiftOn=0;
    static int32 altOn=0;
    static int32 capsOn=0; /* during kernelInit initialise num,caps,scroll */
    static int32 ctrlOn=0;
    int32 isRelease = data & 0x80;
    data &= 0x7F;
    if (data > 0x7F) return ERR_UNKNOWN_KEYCODE;
    if (isRelease && lowKeys[data] < 0x7F)  /* isASCII */
    {
        if (shiftOn ^ capsOn) printk("%c",  highKeys[data]);
        else printk("%c", lowKeys[data]);
    }
    if ((lowKeys[data] == SPECIAL) || (lowKeys[data] == EMPTY))
    {
        switch(data)
        {
            case KEY_LSHIFT:
            case KEY_RSHIFT:
                if (isRelease) shiftOn = 0;
                else shiftOn = 1;
                break;
            case KEY_LALT:
                if (isRelease) altOn = 0;
                else altOn = 1;
                break;
            case KEY_LCTRL:
                if (isRelease) ctrlOn = 0;
                else ctrlOn = 1;
                break;
            case KEY_TAB:
                if (isRelease) printk("    ");
                break;
            case KEY_CAPS:
                if (isRelease) capsOn=0;
                else capsOn=1;
                break;
        }
    }
    return OK;
}
void keyboardIRQ(void)
{
    uint32 cmd, data, e0=0;
    do
    {
        __asm__(
            "mov $(0x64), %%edx\n\t"
            "inb %%dx, %%al\n\t"
            :"=a"(cmd)
            :
        );
        if (cmd & 0x1)
        {
            e0=0;
            {
                __asm__(
                "mov $0x60, %%edx\n\t"
                "inb %%dx, %%al\n\t"
                   :"=a"(data)
                   :
                    );
                e0++;
            } while (data == 0xe0);
            if (e0 != 1) data |= 0x80;
            processKeys(data);
        }
    } while (cmd & 0x1);
}

int archConsoleInit(void)
{
    archRegisterIRQHandler(1, keyboardIRQ);
    return TRUE;
}

int cur_row=4, cur_col=0;
void scroll(void)
{
	char *video = (char *)0xB8000;
	char *src = video + 160;
	/* This code is smaller that what
	 * C compiler managed to generate @ O2.*/
	__asm__
	(
	 "cld\n\t"
	 "movl %0, %%esi\n\t"
	 "movl %1, %%edi\n\t"
	 "movl $(24*40), %%ecx\n\t"
	  "repz\n\t"
	  "movsd\n\t"
	 : 
	 : "m" (src), "m" (video)
	);
	
}


int kputs(char *str)
{
	char *video = (char *)0xB8000;
	while (*str!='\0')
	{
		if (*str == '\n')
		{
			cur_row++;
			cur_col=0;
			str++;
		}
		else 
		{
			*(video+cur_row*160+2*cur_col++) = *str++;
			if (cur_col==80) 
			{
				cur_col=0; cur_row++;
			}
		}
		if (cur_row==25)
		{
			int i;
			cur_row=24; cur_col=0;
			scroll();
			i=0;
			do
			{
				*(video+24*160+i) = ' ';
				*(video+24*160+i+1) = 7;
				i+=2;
			} while (i<160);
		}
	}
	return OK;
}

