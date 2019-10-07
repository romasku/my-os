section .test
global in_byte:function
global in_word:function
global in_dword:function
global out_byte:function
global out_word:function
global out_dword:function
global io_wait:function

in_byte:
	mov 	dx, [esp + 4]
	in		al, dx
	ret

in_word:
	mov 	dx, [esp + 4]
	in		ax, dx
	ret

in_dword:
	mov 	dx, [esp + 4]
	in		eax, dx
	ret

out_byte:
	mov 	dx, [esp + 4]
	mov		al, [esp + 8]
	out		dx, al
	ret

out_word:
	mov 	dx, [esp + 4]
	mov		ax, [esp + 8]
	out		dx, ax
	ret

out_dword:
	mov 	dx, [esp + 4]
	mov		eax, [esp + 8]
	out		dx, eax
	ret

io_wait:
		jmp sec
sec:	jmp thd
thd:	ret
