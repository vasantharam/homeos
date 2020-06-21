#define __ASSEMBLY__
; Boot Loader 
; Objective of the boot loader is to load
; specified number of sectors into INITSEG and
; transfer control to the loaded code.
; 
; The CHS for the partition containing the OS
; ,the number of sectors to load and drive number 
; are part of the Boot Sector and is kept at the end of the sector.

INITSEG	EQU	0x1000 ; We are going to load setup @ absolute addr 0x10000
STACK	EQU	0x9000 ; 64-K stack from 0x90000 - 0x9ffff
NOSECS	EQU	127 ;  mainly because of the dolphin image, else the kernel is only around 14 sectors.

.text
.globl _main
_main:
; Number of sectors and LBA of the setup sector are part
; of the boot sector. Assumptions: BIOS supports LBA.
    mov ax, #0x7c0	; Boot sector segment address
    mov ds, ax		; Current Data Seg, The bios loads us here.
    mov si, #msg
    call prtstr
    mov ax, #INITSEG
    mov es, ax
    mov ax, #STACK
    mov ss, ax
    mov sp, #0xffe0
;    xor dx, dx
;    xor ax, ax
;    int 0x13 		; Reset FDC
;    xor ax, ax
;    mov dx, #0x80        ; Reset first HDD
;    int  0x13
  
;/    LBA read
lba_read:
    xor ax, ax
    mov ah, #0x42 ; service INT13:42 ; IBM/MS Extended LBA read
    mov si, #packet
;/    Normal read alternative, chose other, if problems.
;    xor dx, dx
;    mov dl, #0x80 ;  try Floppy 0x0 first. 
;tryHDD:
;    mov ch, #1 
;    mov cl, #1 ; sector=2
;    xor bx, bx	; Address zero in INITSEG
;    mov ah, #0x2  ; Read
;    mov al, #NOSECS

    int #0x13
    
; If read passed jump to setup.!!
    jnc setup_start
   here: 
    mov al, ah
    mov ah, #0
    add al, #0x41
    mov msg2, al
    mov si, #msg2
    call prtstr

;   int 0x19; Trying to fail boot.
   jmp here
setup_start:
;    mov dx, #0x80 
;    xor ax, ax
;    int 0x13 		; Reset FDC
    JMPI 0, #INITSEG
; putchar
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

.org 390
msg2:
.asciz "LOADFAIL"
.org 400
msg: 
.asciz "Home OS:boot:Loading....."

cur_row:
.word 0
cur_col:
.word 0

.org 491
drive:
.byte 0x80	; 0x80 is the first hard disk.
packet:
.byte 0x10	; packet size
.byte 0		; reserved = 0
packet_nosecs:
.word NOSECS	; no of sectors
packet_offset:
.word 0		; offset
packet_segment:
.word INITSEG	; segment
packet_sector:
.long 1	; Fill a proper LBA address here, to read kernel from LBA disks.
.long 0	; 

.org 510
.word 0xAA55

