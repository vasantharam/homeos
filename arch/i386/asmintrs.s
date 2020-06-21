#define ASSEMBLY

.MACRO GOBACK 
    pop %esi
    pop %edi
    pop %ebp
    pop %ds
    pop %es
    popal
    addl $4, %esp; 
    sti
    iret; 
.ENDM
msg0:
.asciz "I0nterrupt0 \n"
msg1:
.asciz "I1nterrupt0 \n"
msg2:
.asciz "I2nterrupt0 \n"
msg3:
.asciz "I3nterrupt 0\n"
msg4:
.asciz "SOFTWARE TRIGGERED INTERRUPT 0\n"
msg5:
.asciz "I5nterrupt 0\n"
msg6:
.asciz "I6nterrupt 0\n"
msg7:
.asciz "I7nterrupt 0\n"
msg8:
.asciz "I8nterrupt 0\n"
msg9:
.asciz "I9nterrupt 0\n"
msg10:
.asciz "I10nterrupt 0\n"
msg11:
.asciz "I11nterrupt 0\n"
msg12:
.asciz "I12nterrupt 0\n"
msg13:
.asciz "GENERAL PROTECTION FAILURE 0\n"
msg14:
.asciz "I14nterrupt 0\n"
msg15:
.asciz "I15nterrupt 0\n"
msg33:
.asciz "Keyboard Interrupt \n"
msg16:
.asciz "Interrupt 16 0\n"
msg32:
.asciz "Timer interrupt\n"

.globl intr0
 intr0:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg0, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp
    GOBACK
.globl intr1
 intr1:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg1, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp

    GOBACK
.globl intr2
 intr2:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg2, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp

    GOBACK
.globl intr3
 intr3:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg3, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp

    GOBACK
.globl intr4
 intr4:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg4, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp

    GOBACK
.globl intr5
 intr5:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg5, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp

    GOBACK
.globl intr6
 intr6:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg6, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp

    GOBACK
.globl intr7
 intr7:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg7, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp

    GOBACK
.globl intr8
 intr8:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg8, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp

    GOBACK
.globl intr9
 intr9:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg9, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp

    GOBACK
.globl intr10
 intr10:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg10, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp

    GOBACK
.globl intr11
 intr11:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg11, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp

    GOBACK
.globl intr12
 intr12:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg12, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp

    GOBACK
.globl intr13
 intr13:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg13, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp

    a: jmp a
    GOBACK
.globl intr14
 intr14:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg14, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp

    GOBACK
.globl intr15
 intr15:   
    cli
    pushal
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    movl $msg15, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp

    GOBACK
.globl keyboardInterrupt
 keyboardInterrupt:   
    cli
    push %eax
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    push $1
    call doIRQ
    addl $4, %esp
    movl $100, %ecx
   prin_loop:
    push %ecx
    movl $msg33, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp
    pop %ecx
    loop prin_loop

    pop %esi
    pop %edi
    pop %ebp
    pop %ds
    pop %es
    pop %eax
    sti
    iret; 
.globl timerInterrupt
 timerInterrupt:   
    cli
    push %eax
    push %es
    push %ds
    push %ebp
    push %edi
    push %esi

    push $0
    call doIRQ
    addl $4, %esp


    movl $msg32, %eax
    push %eax
    call interrupt_interface
        addl $4, %esp
    call scheduleNewTask /*!Timer triggered task scheduler kickup.*/

    pop %esi
    pop %edi
    pop %ebp
    pop %ds
    pop %es
    pop %eax
    sti
    iret; 
