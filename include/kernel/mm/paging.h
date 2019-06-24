#ifndef KERNEL_PAGING_H
#define KERNEL_PAGING_H

#include <stddef.h>

typedef size_t paging_ctx;

struct pages_list {
    size_t count;
    size_t *pages;
};

paging_ctx arch_get_root_paging_ctx();
size_t arch_virtual_to_physical(size_t addr);
//struct pages_list arch_mm_free_pages_after_boot();

#endif //KERNEL_PAGING_H
