#include <kernel/mm/paging.h>
#include <kernel/panic.h>
#include <kernel/kprintf.h>
#include "paging.h"

extern struct page_dir_entry mm_root_dir_paging_table[1024];

struct page_dir_entry *mm_current_dir_table;
struct page_table_entry *mm_current_paging_table;

void setup_paging() {
    mm_current_dir_table = mm_root_dir_paging_table;
    mm_current_paging_table = (struct page_table_entry *) 0xffc00000;
}

paging_ctx arch_get_root_paging_ctx() {
    return (paging_ctx) arch_virtual_to_physical((size_t) mm_root_dir_paging_table);
}

size_t arch_virtual_to_physical(size_t addr) {
    size_t dir_num = addr >> 22;
    size_t entry_num = (addr >> 12) & 0x3ff;
    if (!mm_current_dir_table[dir_num].present) {
        return 0;
    }
    size_t entry_offset = dir_num * 1024 + entry_num;
    if (!mm_current_paging_table[entry_offset].present) {
        return 0;
    }
    return (mm_current_paging_table[entry_offset].data & (~0x3ff)) | (addr & 0x3ff);
}

void arch_do_page_map(size_t page, size_t entry_offset) {
    mm_current_paging_table[entry_offset].data = page;
    mm_current_paging_table[entry_offset].present = 1;
    mm_current_paging_table[entry_offset].writable = 1;
}

void arch_prepare_page_dir(size_t dir_num) {
    size_t page_for_table = alloc_page();
    // We have last page dir mapped to page tables itself
    arch_do_page_map(page_for_table, 1023 * 1024 + dir_num);
    mm_current_dir_table[dir_num].present = 1;
}

void arch_map_page(size_t page, size_t virtual_addr) {
    size_t dir_num = virtual_addr >> 22;
    size_t entry_num = (virtual_addr >> 12) & 0x3ff;
    if (!mm_current_dir_table[dir_num].present) {
        arch_prepare_page_dir(dir_num);
    }
    size_t entry_offset = dir_num * 1024 + entry_num;
    if (mm_current_paging_table[entry_offset].present) {
        panic("Page was already mapped!");
    }
    arch_do_page_map(page, entry_offset);
}

size_t arch_unmap_page(size_t virtual_addr) {
    size_t dir_num = virtual_addr >> 22;
    size_t entry_num = (virtual_addr >> 12) & 0x3ff;
    if (!mm_current_dir_table[dir_num].present) {
        panic("Cannot unmap page: directory is not presented");
    }
    size_t entry_offset = dir_num * 1024 + entry_num;
    if (!mm_current_paging_table[entry_offset].present) {
        panic("Cannot unmap page: page is not presented");
    }
    mm_current_paging_table[entry_offset].present = 0;
    size_t page = mm_current_paging_table[entry_offset].data & (~0x3ff);
    invalidate_page(virtual_addr);
    return page;
}