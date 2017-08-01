%define selector_video (3<<3)

section .text
global put_char
put_char:
    pushad

    mov cl,[esp+36]
    cmp cl,0x0d
    je .return

    push ds
    push es

    mov eax,selector_video
    mov ds,ax
    mov es,ax

    mov edx,0x03d4
    mov al,0x0e
    out dx,al
    inc edx
    in al,dx
    xor ebx,ebx
    mov bh,al

    dec edx
    mov al,0x0f
    out dx,al
    inc edx
    in al,dx
    mov bl,al


    cmp cl,0x0a
    je .is_line_feed


    cmp cl,0x08
    je .is_backspace

    
    mov [ebx*2],cl
    inc ebx
    jmp .check_cursor

.is_backspace:
    dec ebx
    mov word [ebx*2],0x0700
    jmp .set_cursor

.is_line_feed:
    mov ecx,80
    add ebx,ecx
    mov ax,bx
    div cl
    shr eax,8
    sub ebx,eax

.check_cursor:
    cmp ebx,25*80
    jb .set_cursor

    mov esi,2*80
    xor edi,edi
    mov ecx,24*80/2
    cld
    rep movsd

.clear_last_line: 
    mov eax,0x0700_0700
    mov ecx,80/2
.clearing:
    mov [edi],eax
    add edi,4
    loop .clearing
    mov ebx,24*80

.set_cursor:
    dec edx
    mov al,0x0e
    out dx,al
    inc edx
    mov al,bh
    out dx,al

    dec edx
    mov al,0x0f
    out dx,al
    inc edx
    mov eax,ebx
    out dx,al

    pop es
    pop ds
.return:
    popad
    ret
