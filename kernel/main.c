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


int kernel_main(multiboot_info_t *mbd, unsigned int magic) {
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
    interrupts_init();
    uint32_t old_clock = get_timer_clock();
    while (1) {
#if COUNT_TIME == 1
        uint32_t clock = get_timer_clock();
        if ((old_clock != clock) && (clock % 1000 == 0)) {
            kprintf("Seconds from start: %d\n", clock / 1000);
        }
        old_clock = clock;
#endif
    }
}