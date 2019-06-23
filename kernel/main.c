#include <kernel/tty.h>

int kernel_main() {
    terminal_initialize();
    while (1) ;
}