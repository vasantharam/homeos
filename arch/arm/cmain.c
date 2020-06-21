#include "homeOS.h"
#include "console.h"

void cmain(void)
{
    consoleInit();
    printk("Welcome to homeOS in ARM. The development is in progress\n");
    printk("This is message is out through the uart\n");
    printk("Down load again next week to have a working version of homeOS scheduler and more bug fixes to the existing x86 version.\n");
    while(1)
    {
//        printk("cmain infinite");
    }
}

