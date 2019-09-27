#include <kernel/dev/timer.h>
#include <stdint.h>

volatile uint64_t clock = 0;
void increase_clock() {
    clock++;
}

void init_timer() {
    arch_init_timer();
    arch_register_clock_func(increase_clock);
}

uint64_t get_timer_clock() {
    return clock;
}

