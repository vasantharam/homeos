#ifndef __PLATFORM__H
#define __PLATFORM__H

#include "homeOStypes.h"
#include "errorcodes.h"
#ifdef i386
#include "arch/i386/interface-include/irq.h"
#include "arch/i386/interface-include/io.h"
#include "arch/i386/interface-include/interrupts.h"
#include "arch/i386/interface-include/tasks.h"
#include "arch/i386/interface-include/console.h"
#endif
#ifdef arm
#include "arch/arm/interface-include/irq.h"
#include "arch/arm/interface-include/io.h"
#include "arch/arm/interface-include/interrupts.h"
#include "arch/arm/interface-include/tasks.h"
#include "arch/arm/interface-include/console.h"
#endif
#include "utilities.h"
#endif
