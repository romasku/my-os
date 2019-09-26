; As each interrupt target (vector or more simply speaking, number)
; need it's own handler, so we create all of them here
; All of them do nothing more then save current state, call c-function,
; restore state and exit
; We need 0-20 interrupts for CPU errors and 32-47 for IRQ

; TODO: think more about storing FPU state
section .data
global handlers_array
handlers_array: dd handlers_array_addr
handlers_array_addr:
%assign i 0
%rep    21
dd		handler_%[i]
%assign i i+1
%endrep
%assign i 32
%rep    16
dd		handler_%[i]
%assign i i+1
%endrep

section .text

extern c_interupt_handler

%macro handler 1
global handler_%1:function
handler_%1:
	pusha
	cld
	sub  	esp, 12
	push 	DWORD %1
	call 	c_interupt_handler
	add  	esp, 16
	popa
	iret
%endmacro

%assign i 0
%rep    21
handler i
%assign i i+1
%endrep

%assign i 32
%rep    16
handler i
%assign i i+1
%endrep