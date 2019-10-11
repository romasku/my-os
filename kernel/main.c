#include <kernel/tty.h>
#include <kernel/mm/mm.h>
#include <kernel/multiboot.h>
#include <kernel/panic.h>
#include <kernel/int/interrupts.h>
#include <kernel/dev/timer.h>
#include <kernel/kprintf.h>
#include <kernel/dev/i8042.h>
#include <kernel/dev/ps2_kbd.h>
#include <kernel/config.h>
#include <kernel/bus/pci.h>
#include <kernel/dev/block/ata.h>
#include <kernel/lib/malloc.h>
#include <kernel/scheduling/tasks.h>


void kernel_main(multiboot_info_t *mbd, unsigned int magic) {
    /* Initialize terminal interface */
    tty_initialize();

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("MULTIBOOT BOOTLOADER MAGIC is invalid!\n");
    }
    mm_init(mbd);
    init_timer();
    init_i8042();
    init_ps2_keyboard();
    init_pci_bus();
    init_ata();
    interrupts_init();
    tasks_init();
}