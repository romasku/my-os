#include <kernel/mm/mm.h>
#include <kernel/mm/paging.h>
#include <kernel/kprintf.h>
#include <stdint.h>

void mm_init() {
    arch_mm_init();
    kprintf("Memory manager initialized\n");
}