#include <kernel/tty.h>
#include <kernel/mm/mm.h>
#include <kernel/multiboot.h>
#include <kernel/panic.h>
#include <kernel/int/interrupts.h>

int kernel_main(multiboot_info_t* mbd, unsigned int magic) {
    /* Initialize terminal interface */
    tty_initialize();

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("MULTIBOOT BOOTLOADER MAGIC is invalid!\n");
    }
    mm_init(mbd);
    interrupts_init();
    while (1) ;
}