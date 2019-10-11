; Declare constants for the multiboot header. */
page_align 		equ		1<<0
mem_info 		equ   	1<<1
multiboot_flags	equ 	page_align | mem_info
multiboot_magic equ		0x1BADB002
multiboot_check	equ 	-(multiboot_magic + multiboot_flags)

section		.multiboot
align		4
dd 			multiboot_magic
dd			multiboot_flags
dd			multiboot_check  ; This is checksum

; Exported symbols
global mm_root_dir_paging_table
global stack_top

; Space for stack
section 	.bss
align 		16
stack_bottom:
resb 		16384 ; 16 KiB
stack_top:

virtual_start equ 0xC0000000

extern		_init
extern		_fini
extern		kernel_main


section		.root_paging
mm_root_dir_paging_table:
            ; Initial mapping
            ; first physical 16 mb is mapped to first [0, 16mb] and [3gb, 3gb+16mb] virtual
            dd 		(tmp_page_entries + 4096 * 0 - virtual_start) + 0b00000000011
            dd 		(tmp_page_entries + 4096 * 1 - virtual_start) + 0b00000000011
            dd 		(tmp_page_entries + 4096 * 2 - virtual_start) + 0b00000000011
            dd 		(tmp_page_entries + 4096 * 3 - virtual_start) + 0b00000000011
            resb 	(768 - 4) * 4   ; Skip next 3 gb
            dd 		(tmp_page_entries + 4096 * 0 - virtual_start) + 0b00000000011
            dd 		(tmp_page_entries + 4096 * 1 - virtual_start) + 0b00000000011
            dd 		(tmp_page_entries + 4096 * 2 - virtual_start) + 0b00000000011
            dd 		(tmp_page_entries + 4096 * 3 - virtual_start) + 0b00000000011
            resb 	(256 - 5) * 4
            ; One more page directory (will be mapped to directory content itself to easier life in future)
            dd 		(tmp_page_entries + 4096 * 4 - virtual_start) + 0b00000000011
tmp_page_entries:
            ; Allocate 4 page dirs - 16mb should be enough
            %assign i 0
            %rep    4 * 1024
            ; This is page num to addr is
            dd		i << 12 | 0b00000000011
            %assign i i+1
            %endrep
            ; One more directory for page table it self
            ; TODO: document hack going on here
            %assign i 0
            %rep    4
            dd		(tmp_page_entries + (i << 12) - virtual_start) + 0b00000000011
            %assign i i+1
            %endrep
            resb 	(768 - 4) * 4
            %assign i 0
            %rep    4
            dd		(tmp_page_entries + (i << 12) - virtual_start) + 0b00000000011
            %assign i i+1
            %endrep
            resb 	(256 - 5) * 4
            dd		(tmp_page_entries + (4 << 12) - virtual_start) + 0b00000000011



section		.text
global 		_start:function (_start.end - _start)
global		start:function
_start:
            ; Load Paging Dirs
			mov 	ecx, mm_root_dir_paging_table - virtual_start
			mov 	cr3, ecx
			; Set cr0 paging flag
			mov 	ecx, cr0
			or 		ecx, 1 << 31
			mov 	cr0, ecx
			; Jump to main code in virtual address
			mov 	ecx, after_load
			jmp		ecx
after_load:
			; Setup stack
			mov esp, stack_top
			; Save multiboot params
			sub esp, 8
			push eax
			push ebx

			; Low level init

			; End of low level init
			call _init ; init call - setup C Runtime
			call kernel_main
            call _fini ; finalize C Runtime

			cli
hlt_line:	hlt
			jmp hlt_line ; Inifinity loop if we have been waked up for some reason
_start.end:
