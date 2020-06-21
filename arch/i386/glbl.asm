; Th efile has global entry into the protected mode.
; This is THE starting point for the 32 bit protected
; code.

USE32
INITSEG EQU 0x10000
.globl _main
_main:
global_main: ; The address is referenced directly
             ; from strt.asm code the absolute address
             ; is 0x10200.
    mov eax, #0x18 ; setup DS, ES, FS, GS, SS
    mov ds, ax
    mov es, ax   
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov eax, #0x90000
    mov esp, eax ; Initial stack pointer @ 9ffff
                 ; This is the Global Stack (Hard Coded) for the OS.
    xor eax, eax ; clear eax
    push eax     ; clear all flags with a push pop
    popfd
    ; Now set up the IDT table to better values.
    xor bx, bx
    
    mov eax, #idt ; consider offset considerations
    add eax, #INITSEG+0x200
    mov edx, eax
    mov esi, eax
    xor edi, edi
    cld
idt_copy_loop:
    mov ecx, #2
    mov esi, edx
one_entry:
    movsd
    loop one_entry
    inc bx
    cmp bx, #60
    jnz idt_copy_loop

    mov EAX, #INITSEG+0x200
    add EAX, #msg
    mov ESI, EAX
    call prtstr
    sti
    JMPI #(INITSEG+0x400), #0x10
; Function: prtint
; To print a 32 bit value in Hex format at the current
; location. The input is in register EBX.
prtint:
    PUSH EAX
    PUSH ECX
    PUSH DX
    MOV ECX, EBX
    XOR DX, DX
shift_loop: ; Keep shifting ECX nibbles right, till zero
            ; Print the last 4 bits each time
     MOV EAX, ECX
     AND EAX, #0xF0000000
     ROL EAX, #4
     CMP AL, #0xA
     JNC hex
     ADD EAX, #48 ;ascii value of zero
     JMP nohex
hex:
     ADD EAX, #55
nohex:
     CALL prtchr
     SHL ECX, #4
     INC DX
     CMP DX, #8
     JZ prtint_out
    JMP shift_loop
prtint_out:
    POP DX
    POP ECX
    POP EAX
    RET
; End of prtint

; Function: prtstr
; To print a string at the current location.
; DS: ESI needs be the location of the string.
; Uses: prtchr
prtstr: lodsb
        and     al,al
        jz      fin
        call    prtchr
        jmp     prtstr
fin:    ret
; prtstr end

; Function: prtchr
; To print a character at the current position
; Assumes VGA color support. Base Address: 0xB8000 (absolute)
; Doesnot preserver EAX value
prtchr:
    CMP AL, #65
    JZ do_new_line
    CMP AL, #10
    JZ do_new_line
    CMP AL, #13
    JZ do_new_line
    PUSH EDI
    PUSH EBX
    MOV BL, AL
    XOR EAX, EAX
    INC EAX
    IMUL AX, WORD [INITSEG+0x200+cur_row], #160 ; consider not using 0x10200 here
    ADD AX, WORD [INITSEG+0x200+cur_col]
    ADD AX, WORD [INITSEG+0x200+cur_col]
    ADD EAX, #0xB8000
    MOV EDI, EAX
    SEG FS
    MOV [EDI], BL
    INC WORD  [INITSEG+0x200+cur_col]
    POP EBX
    POP EDI
    RET
do_new_line:
    INC WORD [INITSEG+0x200+cur_row]
    MOV WORD [INITSEG+0x200+cur_col], #0
    RET
; END OF prtchr.

; Function: isr_default
; This function handles all the processor
; exceptions. It prints all the register values and
; returns.

isr_default:
   ; Add some display here later.
  pushad
  push esi
  mov eax, #INITSEG+0x200
  add eax, #interruptmsg
  mov esi, eax 
  pop esi
  popad
  add esp, #4 ;/* This is not the trap type interrupts, that have an error code pushed on the stack. */
  IRET
; END OF isr_default


idt:
.word 0x200+isr_default
.word 0x10 ; selector
.word 0x8e00 ; The above two together contain the P, D, DPL bits..
.word 1 ; offset high word

msg:
.asciz "Welcome To PROTECTED MODE of operation!\n\r"
interruptmsg:
.asciz "Interrupt occured\n\r"

cur_row:
.word 20 
cur_col:
.word 0

.org 511
.byte 0
