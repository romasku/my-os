#ifndef KERNEL_PAGING_H
#define KERNEL_PAGING_H

#include <stddef.h>
#include <kernel/multiboot.h>
#include <stdint.h>

#define PAGE_SIZE 0x1000
#define align_page_lower(n)  (n & ~(PAGE_SIZE - 1))
#define align_page_top(n)  align_page_lower(n + PAGE_SIZE - 1)

typedef size_t paging_ctx;

void paging_init(multiboot_info_t *mbd, void **kernel_heap_start);
void free_page(size_t page);
size_t alloc_page();
size_t free_pages_cnt();

paging_ctx arch_get_root_paging_ctx();
size_t arch_virtual_to_physical(size_t addr);
void arch_map_page(size_t page, size_t virtual_addr);
size_t arch_unmap_page(size_t virtual_addr);

#endif //KERNEL_PAGING_H
