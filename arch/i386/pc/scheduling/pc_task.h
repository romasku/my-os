#ifndef MYOS_PC_TASK_H
#define MYOS_PC_TASK_H

struct arch_task_data {
    void *esp; // Kernel stack
    void *eip; // Kernel instruction pointer
    uint32_t cr3; // Task page root
};

extern void *stack_top;

#endif //MYOS_PC_TASK_H
