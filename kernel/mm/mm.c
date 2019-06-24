#include <kernel/mm/mm.h>
#include <kernel/mm/paging.h>
#include <kernel/kprintf.h>
#include <stdint.h>
#include <kernel/multiboot.h>
#include <kernel/panic.h>

void *kernel_heap_start;
void *kernel_brk_addr;

void mm_init(multiboot_info_t* mbd) {
    arch_mm_init();
    paging_init(mbd, &kernel_heap_start);
    kernel_brk_addr = kernel_heap_start;
    kprintf("Memory manager initialized\n");
    kprintf("System has %u bytes of free space (%umb)\n", mm_free_mem(), mm_free_mem() / 1024 / 1024);
}

size_t mm_free_mem() {
    return free_pages_cnt() * PAGE_SIZE;
}

void kbrk(void *addr) {
    void *new_addr = (void *)align_page_top((size_t)addr);
    if (new_addr < kernel_heap_start) {
        panic("Cannot more brk addr below kernel_heap_start");
    }
    while (kernel_brk_addr < new_addr) {
        // We have to allocate and map page
        size_t page = alloc_page();
        arch_map_page(page, (size_t) kernel_brk_addr);
        kernel_brk_addr += PAGE_SIZE;
    }
    while (new_addr < kernel_brk_addr) {
        // We have to free page
        kernel_brk_addr -= PAGE_SIZE;
        size_t page = arch_unmap_page((size_t) kernel_brk_addr);
        free_page(page);
    }
}

void *ksbrk(size_t increment) {
    void *new_addr = kernel_brk_addr + increment;
    kbrk(new_addr);
    return new_addr;
}