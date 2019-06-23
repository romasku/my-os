#ifndef ARCH_TTY_H
#define ARCH_TTY_H

#include <stddef.h>

void arch_tty_enable_mode(size_t mode);
void arch_tty_putchar(char ascii, size_t x, size_t y);
void arch_tty_move_cursor(size_t x, size_t y);

#endif //ARCH_TTY_H
