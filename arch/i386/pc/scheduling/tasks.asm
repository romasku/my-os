global task_switch_asm:function
global start_task_asm:function

extern complete_task_switch

; void task_switch_asm(uint32_t *old_esp, uint32_t *old_eip, uint32_t new_esp, uint32_t new_eip, struct task *next_task)
task_switch_asm:
    push ebx
    push esi
    push edi
    push ebp
    mov edx, [esp + 16 + 16]     ; Store new_eip to register
    mov ecx, [esp + 16 + 20]     ; Store addr of next_task in register
    mov eax, [esp + 16 + 4]      ; Load addr to store esp
    mov [eax], esp               ; Store current esp
    mov eax, [esp + 16 + 8]      ; Load addr to store eip
    mov [eax], dword going_back  ; Store location to continue this task
    mov eax, [esp + 16 + 12]     ; Load value of new esp
    mov esp, eax            ; Load new esp (We are on new stack!)
    push ecx                ; Move *next_task as parameter to stack
    push edx                ; Prepare to start new task (cpu will jump on next ret)
    jmp complete_task_switch
going_back:
    add esp, 0x4
    pop ebp
    pop edi
    pop esi
    pop ebx
    ret