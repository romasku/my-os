#include <kernel/mm.h>
#include <kernel/kprintf.h>

void mm_init() {
    arch_mm_init();
    kprintf("Memory manager initialized\n");
}