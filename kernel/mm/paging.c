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

void paging_init(multiboot_info_t* mbd) {
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
}


void free_page(size_t page) {
    // We can safely move to the top here - pages of this stack will never go to the free pages
    *pages_stack_top = page;
    //kprintf("Current top: %u\n", pages_stack_top);
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