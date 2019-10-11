#ifndef MYOS_TASKS_H
#define MYOS_TASKS_H

#include <stdint.h>

#define MAX_TASKS 256
#define TICKS_PER_TASK 100

struct arch_task_data;

#define TASK_STATE_IDLE 0
#define TASK_STATE_RUNNING 1
#define TASK_STATE_WAITING 2

struct task {
    uint32_t pid;
    uint8_t state;
    uint8_t ticks_left;
    struct arch_task_data *arch_data;
};

struct task *current_task;
struct task *tasks[MAX_TASKS];

void tasks_init();
void do_scheduling(); // Change context to next awaiting task
void complete_task_switch(struct task *now_running); // Should be called by arch after stack swap


#endif //MYOS_TASKS_H
