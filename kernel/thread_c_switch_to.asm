[bits 32]
global switch_to
section .text
switch_to:
	mov eax,[esp+4]
	mov [eax],esp

	mov eax,[esp+8]
	mov esp,[eax]

	ret
