extern put_char

section .text
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
