section .text
global load_gdt:function
global reload_segments:function
global _enable_tss:function
extern mm_gdtr_descriptor

load_gdt:
    lgdt    [mm_gdtr_descriptor]
    ret

reload_segments:
    jmp     0x08:after_cs_reload
after_cs_reload:
    mov     ax, 0x10
   	mov		ds, ax
   	mov 	es, ax
   	mov		fs, ax
   	mov		gs, ax
   	mov		ss, ax
   	ret

_enable_tss:
   mov 	    ax, [esp + 4]
   ltr      ax
   ret