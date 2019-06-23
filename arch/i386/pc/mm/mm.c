#include <kernel/mm.h>
#include <kernel/kprintf.h>
#include "gdt/gdt.h"

void arch_mm_init() {
    kprintf("Setting new gdt...");
    setup_gdt();
    kprintf(" OK\n");
}