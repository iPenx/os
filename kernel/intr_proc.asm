%define ERROR_CODE nop
%define NO_ERROR_CODE push 0

extern put_int
%macro intr_proc 2
section .text
intr_%1_proc:
    %2
    pushad
    push ds
    mov ax,0x10
    mov ds,ax

    mov eax,0x20
    out 0xa0,al
    out 0x20,al
    
    push dword %1 
    call [intr_proc_table+%1*4]
    jmp interrupt_exit

section .data
    dd intr_%1_proc
%endmacro

extern intr_proc_table

section .text
interrupt_exit:
    add esp,4
    pop ds
    popad
    add esp,4
    iret

section .data
global iet
iet:

intr_proc 0x00,NO_ERROR_CODE
intr_proc 0x01,NO_ERROR_CODE
intr_proc 0x02,NO_ERROR_CODE
intr_proc 0x03,NO_ERROR_CODE
intr_proc 0x04,NO_ERROR_CODE
intr_proc 0x05,NO_ERROR_CODE
intr_proc 0x06,NO_ERROR_CODE
intr_proc 0x07,NO_ERROR_CODE
intr_proc 0x08,ERROR_CODE
intr_proc 0x09,NO_ERROR_CODE
intr_proc 0x0a,ERROR_CODE
intr_proc 0x0b,ERROR_CODE
intr_proc 0x0c,ERROR_CODE
intr_proc 0x0d,ERROR_CODE
intr_proc 0x0e,ERROR_CODE
intr_proc 0x0f,NO_ERROR_CODE
intr_proc 0x10,NO_ERROR_CODE
intr_proc 0x11,ERROR_CODE
intr_proc 0x12,NO_ERROR_CODE
intr_proc 0x13,NO_ERROR_CODE
intr_proc 0x14,NO_ERROR_CODE
intr_proc 0x15,NO_ERROR_CODE
intr_proc 0x16,NO_ERROR_CODE
intr_proc 0x17,NO_ERROR_CODE
intr_proc 0x18,NO_ERROR_CODE
intr_proc 0x19,NO_ERROR_CODE
intr_proc 0x1a,NO_ERROR_CODE
intr_proc 0x1b,NO_ERROR_CODE
intr_proc 0x1c,NO_ERROR_CODE
intr_proc 0x1d,NO_ERROR_CODE
intr_proc 0x1e,NO_ERROR_CODE
intr_proc 0x1f,NO_ERROR_CODE
intr_proc 0x20,NO_ERROR_CODE
