#ifndef KERNEL_MM_H
#define KERNEL_MM_H

#include <stddef.h>

#define KERNEL_MEM_OFFSET 0xc0000000

void mm_init();
size_t mm_free_mem();
void arch_mm_init();

#endif //KERNEL_MM_H
