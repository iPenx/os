%define selector_video (3<<3)
section .txt
global cls
cls:
    pushad
    push ds

    mov ax,selector_video
    mov ds,ax
    
    mov ebx,0
    mov eax,0x0700_0700
    mov ecx,25*80/2
.clearing:
    mov [ebx],eax
    add ebx,4
    loop .clearing

    mov edx,0x03d4
    mov eax,0x0e
    out dx,al
    inc edx
    xor eax,eax
    out dx,al

    dec edx
    mov eax,0x0f
    out dx,al
    inc edx
    xor eax,eax
    out dx,al

    pop ds
    popad
    ret
