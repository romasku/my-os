#include <kernel/tty.h>

int kernel_main() {
    tty_initialize();
    while (1) ;
}