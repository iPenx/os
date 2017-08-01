%define selector_video (3<<3)

section .text
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

global put_str
put_str:
    push ebx
    push ecx

    xor ecx,ecx
    mov ebx,[esp+12]
.checking:
    mov cl,[ebx]
    cmp ecx,0
    je .return
    push ecx
    call put_char
    add esp,4
    inc ebx
    jmp .checking
.return:
    pop ecx
    pop ebx
    ret

section .data
hex db "0123456789ABCDEF"

section .text
global put_int
put_int:
    pushad

    mov ebp,esp
    mov eax,[ebp+9*4]

    cmp eax,0
    je .put_zero
    mov ecx,8
.skip_zero:
    rol eax,4
    mov ebx,eax
    and ebx,0x0000_000f
    cmp ebx,0
    jne .skip_done
    loop .skip_zero

.skip_done:
    push dword [hex+ebx]
    call put_char
    add esp,4
    rol eax,4
    mov ebx,eax
    and ebx,0x0000_000f
    loop .skip_done
    jmp .return

.put_zero:
    push dword '0'
    call put_char
    add esp,4

.return:
    popad
    ret
