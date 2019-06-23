#include <kernel/kprintf.h>
#include "gdt.h"

// TODO: read&learn more about TSS (Task state segment)

/**
 * Five entries:
 *  - null segment
 *  - code level 0
 *  - data level 0
 *  - code level 3
 *  - data level 3
 */
gdt_entry gdt_table[5];
struct gdt_descriptor mm_gdtr_descriptor;

void setup_gdt() {
    struct gdt_entry_data code_seg = {
            .present = 1,
            .base = 0,
            .limit = 0x000FFFFF,
            .limit_is_in_pages = 1,
            .executable = 1,
            .code_readable = 1,
            .size_32_bit = 1,
            .privilege_level = 0
    };
    struct gdt_entry_data data_seg = {
            .present = 1,
            .base = 0,
            .limit = 0x000FFFFF,
            .limit_is_in_pages = 1,
            .executable = 0,
            .data_writable = 1,
            .size_32_bit = 1,
            .privilege_level = 0
    };
    write_gdt_entry(code_seg, gdt_table + 1);
    write_gdt_entry(data_seg, gdt_table + 2);
    // Then we create same entries for user code (privilege_level = 3)
    code_seg.privilege_level = 3;
    data_seg.privilege_level = 3;
    write_gdt_entry(code_seg, gdt_table + 3);
    write_gdt_entry(data_seg, gdt_table + 4);
    mm_gdtr_descriptor.size = sizeof(gdt_table);
    mm_gdtr_descriptor.offset = (uint32_t) gdt_table;
    load_gdt();
    reload_segments();
}