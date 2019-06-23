#include <kernel/tty.h>
#include <kernel/mm.h>

int kernel_main() {
    tty_initialize();
    mm_init();
    while (1) ;
}