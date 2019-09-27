#ifndef OS_ROOT_TIMER_H
#define OS_ROOT_TIMER_H

#include <stdint.h>

#define CLOCK_FREQ 1000

typedef void (*clock_func)(void);

void init_timer();
void arch_init_timer();
void arch_register_clock_func(clock_func);
uint64_t get_timer_clock();

#endif //OS_ROOT_TIMER_H
