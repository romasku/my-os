global lock_acquire_no_block_asm:function

lock_acquire_no_block_asm:
    mov eax, [esp + 4]
    lock bts dword[eax], 0
    jc   .not_acquired
    mov eax, 1
    ret
.not_acquired:
    mov eax, 0
    ret