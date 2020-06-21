; The first objective of the start up code is to get into
; protected mode. After which the scheduler kicks up. Then
; global_init is called which can inturn initializes the
; whole system. global_init and the CS needs USE 32

#define __STARTUP_CS 0x10
INITSEG EQU 0x10000
.globl _main
_main: 
; This point is loaded at address 0x10000
    mov ax, cs
    mov ds, ax
    mov si, #msg ; Remove msg later
    call prtstr
    mov cx, #0xc00
    lo: loop lo
    mov si, #msg
    call prtstr
    lidt idt_48 ; Load Interrupt Descriptor Register.
    lgdt gdt_48 ; Load Global Descriptor Register.
    mov si, #msg
    call prtstr 


    call    empty_8042
    mov     al,#0xD1                ; command write
    out     #0x64,al
    call    empty_8042
    mov     al,#0xDF                ; A20 on
    out     #0x60,al
    call    empty_8042

; Interrupts configuration. Mask All to be safe.
    mov     al,#0x11                ; initialization sequence
    out     #0x20,al                ; send it to 8259A-1
    call    delay
    out     #0xA0,al                ; and to 8259A-2
    call    delay
    mov     al,#0x20                ; start of hardware int's (0x20)
    out     #0x21,al
    call    delay
    mov     al,#0x28                ; start of hardware int's 2 (0x28)
    out     #0xA1,al
    call    delay
    mov     al,#0x04                ; 8259-1 is master
    out     #0x21,al
    call    delay
    mov     al,#0x02                ; 8259-2 is slave
    out     #0xA1,al
    call    delay
    mov     al,#0x01                ; 8086 mode for both
    out     #0x21,al
    call    delay
    out     #0xA1,al
    call    delay
    mov     al,#0xFF                ; mask off all interrupts for now
    out     #0xA1,al
    call    delay
    mov     al,#0xFB                ; mask all irq's but irq2 which
    out     #0x21,al                ; is cascaded

; NOW comes the interesting part of actually switching to THE protected
; mode.
   
    mov     ax, #1
    lmsw    ax ; That was nice. We are in protected mode now !!!
    jmp     flush
flush:

    
; Now the only thing to do in the world is to jump into the 32bit
; code (global_init)
    .byte 0x66, 0xea ; 66: 32 bit prefix. ea: JMPI instruction
    .long INITSEG+0x200     ; address of global_init in glbl.asm
    .word   0x10   ; startup cs
; End Main

; Function : delay
delay:
    .word   0x00eb                  ; jmp $+2
    ret

; Function : empty_8042
; 
empty_8042:
    push    ecx
    mov     ecx,#0xFFFFFF

empty_8042_loop:
    dec     ecx
    jz      empty_8042_end_loop

    call    delay
    in      al,#0x64        ; 8042 status port
    test    al,#1           ; output buffer?
    jz      no_output
    call    delay
    in      al,#0x60        ; read it
    jmp     empty_8042_loop
no_output:
    test    al,#2           ; is input buffer full?
    jnz     empty_8042_loop ; yes - loop
empty_8042_end_loop:
    pop     ecx
    ret
; End emprt_8042

gdt:
        .word   0,0,0,0         ! dummy

        .word   0,0,0,0         ! unused

        .word   0xFFFF          ! 4Gb - (0x100000*0x1000 = 4Gb)
        .word   0x0000          ! base address=0
        .word   0x9A00          ! code read/exec
        .word   0x00CF          ! granularity=4096, 386 (+5th nibble of limit)

        .word   0xFFFF          ! 4Gb - (0x100000*0x1000 = 4Gb)
        .word   0x0000          ! base address=0
        .word   0x9200          ! data read/write
        .word   0x00CF          ! granularity=4096, 386 (+5th nibble of limit)

        .word   tss_end-tss_base
        .word   tss_base+INITSEG
        .word   0x8b00
        .word   0x0010

tss_base:
.space 116
tss_end:

gdt_48:
.word 0x800
.long INITSEG+gdt

idt_48:
.word 512  ; limit = 64 entries
.word 0, 0 ; base =0

prtchr:
CMP BX, #0x10
JZ do_new_line
PUSH FS
PUSH AX
PUSH DI
PUSH BX
MOV BX, AX
MOV AX, #0xB800
MOV FS, AX
IMUL AX, cur_row, #160
ADD AX, cur_col
ADD AX, cur_col
MOV DI, AX
MOV AX, BX
SEG FS
MOV [DI],AL
INC cur_col
POP BX
POP DI
POP AX
POP FS
RET
do_new_line:
INC cur_row
MOV cur_col, #0
RET

;PRINT STRING
; To print DS:SI string
prtstr: lodsb
        and     al,al
        jz      fin
        call    prtchr
        jmp     prtstr
fin:    ret
cur_row:
.word 0
cur_col:
.word 0
msg:
.asciz "HomeOS:strt:Loading ..\n"
.org 511
.byte 0
