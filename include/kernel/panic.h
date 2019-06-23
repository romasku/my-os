#ifndef KERNEL_PANIC_H
#define KERNEL_PANIC_H

void panic_no_tty();
void panic(const char *message);

#endif //KERNEL_PANIC_H
