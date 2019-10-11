#ifndef MYOS_ARCH_TASKS_H
#define MYOS_ARCH_TASKS_H

void arch_init_task(struct task *task, void *entry_eip);
void arch_switch_tasks(struct task *last_task, struct task *next_task);

#endif //MYOS_ARCH_TASKS_H
