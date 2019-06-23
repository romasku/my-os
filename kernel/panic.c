#include <kernel/panic.h>
#include <kernel/kprintf.h>

void panic_no_tty() {
    while(1);
}

void panic(const char *message) {
    kprintf(message);
    while(1);
}
