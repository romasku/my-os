#include <kernel/mm/mm.h>
#include <kernel/mm/paging.h>
#include <kernel/kprintf.h>
#include <stdint.h>
#include <kernel/multiboot.h>

extern char _start_of_kernel;
extern char _end_of_kernel;

void mm_init(multiboot_info_t* mbd) {
    arch_mm_init();
    paging_init(mbd);
    kprintf("Memory manager initialized\n");
    kprintf("System has %u bytes of free space (%umb)\n", mm_free_mem(), mm_free_mem() / 1024 / 1024);
}

size_t mm_free_mem() {
    return free_pages_cnt() * PAGE_SIZE;
}