#ifndef I386_PC_GDT_H
#define I386_PC_GDT_H

#include <stdint.h>
#include <stdbool.h>

struct gdt_descriptor {
    uint16_t size;
    uint32_t offset;
} __attribute__((packed));
struct gdt_entry_data {
    uint32_t base;
    uint32_t limit;
    int present: 1;
    unsigned int privilege_level: 2;
    int type_tss: 1;
    int executable: 1;
    int direction_grows_down: 1;
    int conforming: 1;
    int code_readable: 1;
    int data_writable: 1;
    int accessed: 1;
    int limit_is_in_pages: 1;
    int size_32_bit: 1;
};
typedef uint64_t gdt_entry;

void write_gdt_entry(struct gdt_entry_data data, gdt_entry *entry);
void setup_gdt();

// Next to functions implemented in asm.
extern void load_gdt(); // Simply loads symbol 'mm_gdtr_descriptor' location as gdt
extern void reload_segments(); // Reloads all cpu segments registers

#endif //I386_PC_GDT_H
