#ifndef OS_ROOT_TTY_H
#define OS_ROOT_TTY_H
#include <stddef.h>

void terminal_initialize(void);
void terminal_write(const char *data, size_t size);

#endif //OS_ROOT_TTY_H
