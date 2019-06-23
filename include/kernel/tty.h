#ifndef KERNEL_TTY_H
#define KERNEL_TTY_H
#include <stddef.h>

#define MODE_80x24 1

void tty_initialize(void);
void tty_enable_mode(size_t mode);
void tty_putchar(char ascii);
void tty_write(const char *data, size_t size);
void tty_clear_screen();

#endif //KERNEL_TTY_H
