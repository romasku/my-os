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
    int dir_num = addr >> 22;
    int entry_num = (addr >> 12) & 0x3ff;
    if (!mm_current_dir_table[dir_num].present) {
        return 0;
    }
    int entry_offset = dir_num * 1024 + entry_num;
    if (!mm_current_paging_table[entry_offset].present) {
        return 0;
    }
    return (mm_current_paging_table[entry_offset].data & (~0x3ff)) | (addr & 0x3ff);
}