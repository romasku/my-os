#include <kernel/mm/mm.h>
#include <kernel/kprintf.h>
#include "gdt/gdt.h"
#include "paging/paging.h"

void arch_mm_init() {
    kprintf("Setting new gdt...");
    setup_gdt();
    kprintf(" OK\n");
    kprintf("Preparing x86 paging variables...");
    setup_paging();
    kprintf(" OK\n");
}