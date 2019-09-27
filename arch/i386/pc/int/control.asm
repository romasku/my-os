section .test
global enable_interrupts:function
global load_idt:function

load_idt:
	; First argument - mm location, second is size
	mov 	eax, [esp + 4]
	mov 	[idtr + 2], eax
	mov 	ax, [esp + 8]
	mov 	[idtr], ax
	lidt	[idtr]			; Yes, we have to load this with [],
	ret

enable_interrupts:
	sti
	ret

section .data
idtr:	dw 0
		dd 0