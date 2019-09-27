section .test
global in_byte:function
global out_byte:function
global io_wait:function

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
