%include "boot.inc"

section mbr vstart=0x7c00
start:
    mov ax,cs
    mov es,ax
    mov ds,ax

    mov ax,0x0600
    mov bx,0x0700
    mov cx,0x0000
    mov dx,0x184f
    int 10h
    
    mov bp,msg
    mov cx,len_msg
    mov bx,0x0007
    mov dx,0x0000
    mov ax,0x1301
    int 10h

    mov eax,LOADER_START_SECTOR
    mov bx,LOADER_BASE_ADDR
    mov cx,1
    call readDisk_16bits

    mov ax,[LOADER_BASE_ADDR+LOADER_MAGIC_ADDR]
    cmp ax,LOADER_MAGIC
    jne .error_no_loader 
    mov cx,[LOADER_BASE_ADDR+LOADER_SECTOR_NUM_ADDR]
    dec cx
    cmp cx,0
    je .jmp_loader
    mov eax,LOADER_START_SECTOR+1
    call readDisk_16bits

.jmp_loader:
    jmp [LOADER_BASE_ADDR+LOADER_ENTRY_ADDR]
    
.error_no_loader:
    mov bp,msg01
    mov cx,len_msg01
    mov bx,0x0007
    mov dx,0x0100
    mov ax,0x1301
    int 10h

    jmp $

msg db 'The mbr has been loaded.'
len_msg equ $-msg
msg01 db 'MBR: Error! There is no loader.'
len_msg01 equ $-msg01

readDisk_16bits:
    mov esi,eax
    mov di,cx

    mov dx,0x1f2
    mov al,cl
    out dx,al

    mov eax,esi

    inc dx
    out dx,al

    mov cl,8
    shr eax,cl
    inc dx
    out dx,al

    shr eax,cl
    inc dx
    out dx,al

    shr eax,cl
    and al,0x0f
    or al,0xe0
    inc dx
    out dx,al

    mov al,0x20
    inc dx
    out dx,al

.waiting:
    nop
    in al,dx
    and al,0x88
    cmp al,0x08
    jne .waiting
    
    mov cx,di
    shl cx,8

    mov dx,0x1f0
.reading:
    in ax,dx
    mov [bx],ax
    add bx,2
    loop .reading

    ret

times 510-($-$$) db 0
    db 0x55,0xaa
