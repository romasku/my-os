#include <kernel/scheduling/tasks.h>
#include <kernel/lib/malloc.h>
#include <kernel/mm/paging.h>
#include <kernel/kprintf.h>
#include "pc_task.h"

extern void task_switch_asm(void **old_esp, void **old_eip, void *new_esp, void *new_eip,
                            struct task *next_task);

void arch_init_task(struct task *task, void *entry_eip) {
    task->arch_data = malloc(sizeof(struct arch_task_data));
    task->arch_data->eip = entry_eip;
    task->arch_data->esp = malloc(16384) + 16484;
    task->arch_data->esp += 16 - ((size_t)task->arch_data->esp % 16) + 12;
    task->arch_data->cr3 = arch_get_root_paging_ctx();
}


void arch_switch_tasks(struct task *last_task, struct task *next_task) {
    task_switch_asm(&last_task->arch_data->esp, &last_task->arch_data->eip,
                    next_task->arch_data->esp, next_task->arch_data->eip, next_task);
}