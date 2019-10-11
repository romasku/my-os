#include <kernel/dev/timer.h>
#include <stdint.h>
#include <stddef.h>
#include <kernel/kprintf.h>

#define MAX_INTERVAL_FUNCS 20


volatile uint64_t clock = 0;
uint32_t *sub_clock = &clock;
interval_func interval_funcs[MAX_INTERVAL_FUNCS];

void on_timer_tick() {
    clock++;
    for (int i = 0; i < MAX_INTERVAL_FUNCS; i++) {
        if (interval_funcs[i] != NULL) {
            interval_funcs[i](1000 / CLOCK_FREQ);
        }
    }

}

void init_timer() {
    arch_init_timer();
    arch_register_clock_func(on_timer_tick);
}

void register_interval_func(interval_func func) {
    for (int i = 0; i < MAX_INTERVAL_FUNCS; i++) {
        if (interval_funcs[i] == NULL) {
            interval_funcs[i] = func;
            break;
        }
    }
}

uint64_t get_timer_clock() {
    return clock;
}

