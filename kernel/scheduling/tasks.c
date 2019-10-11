#include <kernel/scheduling/tasks.h>
#include <kernel/lib/malloc.h>
#include <kernel/arch/tasks.h>
#include <kernel/kprintf.h>
#include <kernel/dev/timer.h>

uint32_t tasks_count = 0;

static void task_0_logic() {
    while (1) {
        kprintf("0");
        do_scheduling();
    }
}

static void task_1_logic() {
    while (1) {
        kprintf("1");
    }
}

static void prepare_task_0();
static void prepare_task_1();
static void decrease_ticks(uint32_t milliseconds);

void tasks_init() {
    prepare_task_0();
    prepare_task_1();
    current_task = tasks[0];
    register_interval_func(decrease_ticks);
    task_0_logic();
}

static void prepare_task_0() {
    // prepare task
    tasks[0] = malloc(sizeof(struct task));
    tasks[0]->pid = 0;
    tasks[0]->state = TASK_STATE_RUNNING;
    tasks[0]->ticks_left = TICKS_PER_TASK;
    arch_init_task(tasks[0], task_0_logic);
    tasks_count++;
}

static void prepare_task_1() {
    tasks[1] = malloc(sizeof(struct task));
    tasks[1]->pid = 1;
    tasks[1]->state = TASK_STATE_RUNNING;
    tasks[1]->ticks_left = TICKS_PER_TASK;
    arch_init_task(tasks[1], task_1_logic);
    tasks_count++;
}

static struct task *find_next_task() {
    for (uint32_t index = 0; index < tasks_count; index++) {
        if (tasks[index] != NULL && tasks[index]->state == TASK_STATE_RUNNING && tasks[index]->ticks_left > 0) {
            return tasks[index];
        }
    }
    return NULL;
}

void do_scheduling() {
    struct task *next_task = find_next_task();
    if (next_task == NULL) {
        // Lets reset ticks for all tasks
        for (uint32_t index = 0; index < tasks_count; index++) {
            if (tasks[index] != NULL) {
                tasks[index]->ticks_left = TICKS_PER_TASK;
            }
        }
    }
    next_task = find_next_task();
    if (next_task == NULL) {
        // We still have no task to run, then active first task
        next_task = tasks[0];
    }
    if (current_task != next_task) {
        arch_switch_tasks(current_task, next_task);
    }
}

void complete_task_switch(struct task *now_running) {
    current_task = now_running;
}

static void decrease_ticks(uint32_t milliseconds) {
    // TODO: remove next if when locks are implemented
    if (current_task->ticks_left == 0) {
        return;
    }
    current_task->ticks_left--;
    if (current_task->ticks_left == 0) {
        do_scheduling();
    }
}
