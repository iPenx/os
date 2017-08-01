%include "boot.inc"

section loader vstart=LOADER_BASE_ADDR
LOADER_STACK_TOP equ $
header:
    dw LOADER_MAGIC
    dw (((end_loader-$$)-1)>>9)+1
    dw entry
    dw 0
    dd 32*1024*1024

GDT_BASE:
GDT_NULL dd 0x0000_0000
         dd 0x0000_0000

GDT_CODE dd 0x0000_FFFF
         dd DESC_CODE_HIGH4

GDT_DATA dd 0x0000_FFFF
         dd DESC_DATA_HIGH4

GDT_VIDEO dd 0x8000_0007
          dd DESC_VIDEO_HIGH4

GDT_LIMIT equ $-GDT_BASE-1

times 60 dq 0

selector_code equ (GDT_CODE-GDT_NULL) | TI_GDT | RPL0
selector_data equ (GDT_DATA-GDT_NULL) | TI_GDT | RPL0
selector_video equ (GDT_VIDEO-GDT_NULL) | TI_GDT | RPL0

ptrGDT dw GDT_LIMIT
       dd GDT_BASE


entry:
    mov bp,msg
    mov cx,len_msg
    mov bx,0x0007
    mov dx,0x0100
    mov ax,0x1301
    int 10h

    cli

    in al,0x92
    or al,0000_0010b
    out 0x92,al

    lgdt [ptrGDT]

    mov eax,cr0
    or eax,0x0000_0001
    mov cr0,eax

    jmp dword selector_code:p_mode_start

[bits 32]
p_mode_start:
    mov ax,selector_data
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov ss,ax
    mov esp,LOADER_STACK_TOP

    mov ax,selector_video
    mov gs,ax

    mov ah,0x07
    mov ebx,2*80*2
    xor edi,edi
    mov ecx,len_msg01
.print_msg01:
    mov al,[msg01+edi]
    mov [gs:ebx],ax
    inc edi
    add ebx,2
    loop .print_msg01

load_kernel_bin:
    mov eax,KERNEL_START_SECTOR
    mov ebx,KERNEL_BIN_BASE_PADDR
    mov ecx,KERNEL_BIN_TOTAL_SECTOR
    call readDisk

    
open_virtual_page:
    xor eax,eax
    mov ebx,PAGE_DIR_TABLE_PADDR
    mov esi,0
    mov ecx,1024
.clear_page_dir:
    mov [ebx+esi],eax
    add esi,4
    loop .clear_page_dir

    mov eax,ebx
    or eax,PG_U | PG_RDWR | PG_P
    mov [ebx+(1023<<2)],eax

    add eax,0x1000
    mov [ebx+(0<<2)],eax
    mov [ebx+(768<<2)],eax

    add ebx,0x1000
    mov eax,PG_U | PG_RDWR | PG_P
    mov ecx,256
.setup_low_1MB_PAGE:
    mov [ebx],eax
    add ebx,4
    add eax,0x1000
    loop .setup_low_1MB_PAGE

    sgdt [ptrGDT]

    mov ebx,[ptrGDT+2]
    or dword [ebx+(selector_video & 1111_1000)+4],0xc000_0000

    add dword [ptrGDT+2],0xc000_0000

    add esp,0xc000_0000

    mov eax,PAGE_DIR_TABLE_PADDR
    mov cr3,eax

    mov eax,cr0
    or eax,0x8000_0000
    mov cr0,eax

    lgdt [ptrGDT]

    mov ax,selector_video
    mov gs,ax

    mov ah,0x07
    mov esi,0
    mov ebx,3*80*2
    mov ecx,len_msg02
.print_msg02:
    mov al,[msg02+esi]
    mov [gs:ebx],ax
    add ebx,2
    inc esi
    loop .print_msg02

kernel_init:
    xor ecx,ecx
    xor edx,edx

    mov dx,[KERNEL_BIN_BASE_PADDR+e_phentsize]
    mov ebx,[KERNEL_BIN_BASE_PADDR+e_phoff]
    add ebx,KERNEL_BIN_BASE_PADDR
    mov cx,[KERNEL_BIN_BASE_PADDR+e_phnum]

.load_each_segment:
    cmp dword [ebx+p_type],PT_NULL
    je .PT_NULL
    push dword [ebx+p_filesz]
    mov eax,[ebx+4]
    add eax,KERNEL_BIN_BASE_PADDR
    push eax
    push dword [ebx+p_vaddr]
    call mem_cpy
    add esp,12
.PT_NULL:
    add ebx,edx
    loop .load_each_segment

    mov esp,0xc009_f000
    jmp KERNEL_ENTRY_POINT

msg db 'The loader has been loaded.'
len_msg equ $-msg
msg01 db "It's in 32 bits mode."
len_msg01 equ $-msg01
msg02 db "It has opened the virtual page mode."
len_msg02 equ $-msg02

readDisk:
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi

    mov esi,eax
    mov edi,ecx

    mov dx,0x1f2
    mov al,cl
    out dx,al

    mov eax,esi
    
    inc dx
    out dx,al

    shr eax,8
    inc dx
    out dx,al

    shr eax,8
    inc dx
    out dx,al

    shr eax,8
    and al,0x0f
    or  al,0xe0
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

    mov dx,0x1f0
    mov ecx,edi
    shl ecx,8
.reading:
    in ax,dx
    mov [ebx],ax
    add ebx,2
    loop .reading

    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
    ret

mem_cpy:
    push ebp
    mov ebp,esp

    push ecx
    push esi
    push edi

    mov edi,[ebp+8]
    mov esi,[ebp+12]
    mov ecx,[ebp+16]
    cld
    rep movsb

    pop edi
    pop esi
    pop ecx
    pop ebp
    ret

end_loader:
