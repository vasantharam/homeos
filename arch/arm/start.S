/* Hello world start.S */

#define USER_STACK 0x200000 
/* 2MB  */
/* Stack size of SVC STACK is 64K*/
#define SVC_STACK  0x100000 
/* 1MB */
#define EXCEPTION_STACK 0xF0000 /*; Just below SVC_STACK*/
/* Modes at reset have I, F bits set to disable interrupts*/
#define MODE_FIQ 0x11
#define MODE_IRQ 0x12
#define MODE_SVC 0x13
#define MODE_ABORT 0x17
#define MODE_UNDEF 0x1b
#define MODE_SWI MODE_SVC

.macro SET_MODE_AT_RESET 
	mrs	r0,cpsr
	bic	r0,r0,#0x1f
	orr	r0,r0, r1
	msr	cpsr,r0
.endm

.globl cmain

.globl _start
_start:
	b	reset
	ldr	pc, common_exception_handler  /* Is it possible to use imm directly from label? */
	ldr	pc, common_exception_handler
	ldr	pc, common_exception_handler
	ldr	pc, common_exception_handler
	ldr	pc, common_exception_handler
	ldr	pc, common_exception_handler
	ldr	pc, common_exception_handler

_svc_stack:
    .word SVC_STACK
_exception_stack:
    .word EXCEPTION_STACK

reset:
	/*
	 * Set Stack for each mode
	 */

    mov r1, #MODE_FIQ
    SET_MODE_AT_RESET
    ldr r13, (_exception_stack)

    mov r1, #MODE_IRQ
    SET_MODE_AT_RESET
    ldr r13, (_exception_stack)

    mov r1, #MODE_ABORT
    SET_MODE_AT_RESET
    ldr r13, (_exception_stack)

    mov r1, #MODE_UNDEF
    SET_MODE_AT_RESET
    ldr r13, (_exception_stack)

    mov r1, #MODE_SVC
    SET_MODE_AT_RESET
    ldr sp, (_svc_stack)

    bl coprocessor_init

    /* Lets jump to a C program and hope to never return */
    b cmain



coprocessor_init:
	/*
	 * flush v4 I/D caches
	 */
	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0	/* flush v3/v4 cache */
	mcr	p15, 0, r0, c8, c7, 0	/* flush v4 TLB */

	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002300	/* clear bits 13, 9:8 (--V- --RS) */
	bic	r0, r0, #0x00000087	/* clear bits 7, 2:0 (B--- -CAM) */
	orr	r0, r0, #0x00000002	/* set bit 2 (A) Align */
	orr	r0, r0, #0x00001000	/* set bit 12 (I) I-Cache */
	mcr	p15, 0, r0, c1, c0, 0

	/*
	 * Go setup Memory and board specific bits prior to relocation.
	 */
	mov	pc, lr		/* back to my caller */

/*
 * This is a dummy exception handler
 * 
 */
	.align  5
common_exception_handler:
    stmfd r13!, {r0, lr}
    mrs r0, cpsr
	bic	r0,r0,#(~0x1f)
    cmp r0, #(MODE_IRQ)
    beq irq_ret
    cmp r0, #(MODE_FIQ)
    beq fiq_ret
    cmp r0, #(MODE_ABORT)
    beq abort_ret
    cmp r0, #(MODE_UNDEF)
    beq undef_ret
    cmp r0, #(MODE_SWI)
    beq swi_ret
    /* Something's terribly wrong CPSR mode incorrect */
    /* TODO: Add code to print panic message and HALT */
swi_ret:
undef_ret:
    ldmfd r13!, {r0, lr}
    movs pc, lr
abort_ret: /* prefetch abort and data abort should be handled differently,
              This code would cause prefetch aborts to attempt to continue 
              reexecute the failing instruction.
              And would cause data abort to continue from next instruction.
            */
fiq_ret:
irq_ret:
    ldmfd r13!, {r0, lr}
    subs pc, lr, #4


