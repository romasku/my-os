#ifndef OS_ROOT_TIMER_H
#define OS_ROOT_TIMER_H

#include <stdint.h>

#define CLOCK_FREQ 1000

typedef void (*clock_func)(void);
typedef void (*interval_func)(uint32_t milliseconds_delta);

void init_timer();
void register_interval_func(interval_func func);

void arch_init_timer();
void arch_register_clock_func(clock_func func);
uint64_t get_timer_clock();

#endif //OS_ROOT_TIMER_H
