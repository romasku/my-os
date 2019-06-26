#ifndef KERNEL_MM_H
#define KERNEL_MM_H

#include <stddef.h>
#include <kernel/multiboot.h>

#define KERNEL_MEM_OFFSET 0xc0000000

void mm_init(multiboot_info_t* mbd);
size_t mm_free_mem();
void arch_mm_init();
void kbrk(void *addr);
void* ksbrk(size_t increment);

#endif //KERNEL_MM_H
