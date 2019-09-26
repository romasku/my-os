section .test
global enable_interrupts:function
global load_idt:function
global in_byte:function
global out_byte:function
global io_wait:function

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

in_byte:
	mov 	dx, [esp + 4]
	in		al, dx
	ret

out_byte:
	mov 	dx, [esp + 4]
	mov		al, [esp + 8]
	out		dx, al
	ret

io_wait:
		jmp sec
sec:	jmp thd
thd:	ret

section .data
idtr:	dw 0
		dd 0