global lock_is_locked:function
global lock_acquire:function
global lock_acquire_no_block:function
global lock_release:function

lock_is_locked:
    mov eax, [esp + 4]
    cmp dword[eax], 0
    je .free
    mov eax, 1
    ret
.free:
    mov eax, 0
    ret

lock_acquire:
    mov eax, [esp + 4]
    lock bts dword[eax], 0
    jc   .spin
    ret
.spin:
    pause
    test dword[eax], 1
    jnz .spin
    jmp lock_acquire

lock_acquire_no_block:
    mov eax, [esp + 4]
    lock bts dword[eax], 0
    jc   .not_acquired
    mov eax, 1
    ret
.not_acquired:
    mov eax, 0
    ret

lock_release:
    mov eax, [esp + 4]
    mov dword [eax], 0
    ret