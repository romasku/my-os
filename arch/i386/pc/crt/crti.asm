section		.init
global		_init:function
_init:
			push ebp
			mov  ebp, esp
			; All logic will be linked after this line

section 	.fini
global 		_fini:function
_fini:
			push ebp
            mov  ebp, esp
            ; All logic will be linked after this line

