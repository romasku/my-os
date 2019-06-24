#include <kernel/multiboot.h>
#include <stdint.h>
#include <kernel/mm/mm.h>
#include <stddef.h>
#include <kernel/mm/paging.h>
#include <kernel/panic.h>
#include <kernel/kprintf.h>

extern char _start_of_kernel;
extern char _end_of_kernel;

size_t *pages_stack_bottom;
size_t *pages_stack_top;

int page_part_of_kernel(size_t addr) {
    return ((size_t) &_start_of_kernel < addr && addr < (size_t) &_end_of_kernel);
}

int page_part_of_free_pages_stack(size_t addr, size_t free_pages_cnt_estimate) {
    return ((size_t) &_end_of_kernel < addr && addr < (size_t) &_end_of_kernel + free_pages_cnt_estimate * 4);
}

void paging_init(multiboot_info_t *mbd, void **kernel_heap_start) {
    pages_stack_top = pages_stack_bottom = (size_t *) ((size_t) &_end_of_kernel + KERNEL_MEM_OFFSET);

    kprintf("Preparing stack of free pages... ");
    size_t free_pages_cnt_estimate = (mbd->mem_lower + mbd->mem_upper) * 1024 / PAGE_SIZE;
    uint32_t map_regions_cnt = mbd->mmap_length / sizeof(multiboot_memory_map_t);
    multiboot_memory_map_t *mmaps = (multiboot_memory_map_t *) mbd->mmap_addr + KERNEL_MEM_OFFSET;
    for (uint32_t i = 0; i < map_regions_cnt; i++) {
        if (mmaps[i].type == MULTIBOOT_MEMORY_AVAILABLE && mmaps[i].len > PAGE_SIZE) {
            size_t addr = align_page_top(mmaps[i].addr);
            size_t region_end_addr = mmaps[i].addr + mmaps[i].len;
            while (addr < region_end_addr) {
                if (!page_part_of_kernel(addr) && !page_part_of_free_pages_stack(addr, free_pages_cnt_estimate)) {
                    free_page(addr);
                }
                addr += PAGE_SIZE;
            }
        }
    }
    kprintf("%d pages stored\n", free_pages_cnt());

    *kernel_heap_start = (void *)align_page_top((size_t)(KERNEL_MEM_OFFSET + &_end_of_kernel
                                                         + free_pages_cnt_estimate * 4));

    // Currently, loader have mapped virtual [3gb, 3gb+16mb] to [0,16mb].
    // We are going to use heap properly after this moment, so we have to unmap the rest
    size_t addr_to_unmap = (size_t)*kernel_heap_start;
    while (addr_to_unmap < KERNEL_MEM_OFFSET + 16 * 1024 * 1024) {
        arch_unmap_page(addr_to_unmap);
        addr_to_unmap += PAGE_SIZE;
    }
}


void free_page(size_t page) {
    // We can safely move to the top here - pages of this stack will never go to the free pages
    *pages_stack_top = page;
    pages_stack_top++;
}

size_t alloc_page() {
    if (pages_stack_top == pages_stack_bottom) {
        panic("OUT OF MEMORY. No more free pages, cannot allocate");
    }
    return *(pages_stack_top--);
}

size_t free_pages_cnt() {
    return ((size_t)pages_stack_top - (size_t)pages_stack_bottom) / sizeof(size_t);
}