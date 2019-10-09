#include <stdint.h>
#include <stddef.h>
#include <kernel/kprintf.h>
#include "tss.h"
#include "gdt_segments.h"

uint8_t task_0_stack[16384];

void init_tss() {
    tss_segment.ss0 = system_data_segment;
    tss_segment.esp0 = (size_t)task_0_stack;
}

void activate_tss() {
    _enable_tss(system_tss_segment);
}
