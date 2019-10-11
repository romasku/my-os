#include <stddef.h>
#include <stdint.h>
#include <kernel/kprintf.h>
#include <kernel/lib/string.h>
#include <kernel/int/interrupts.h>
#include "idt.h"
#include "pic.h"
#include "../mm/gdt/gdt_segments.h"

extern uint32_t *handlers_array;
extern void load_idt(struct IDT_entry *idt, size_t size);
extern void enable_interrupts();
struct IDT_entry idt[48];

void c_interrupt_handler(uint8_t interrupt_code) {
    if (32 <= interrupt_code && interrupt_code <= 48) {
        pic_end_of_interrupt((uint8_t) (interrupt_code - 32));
    }
    base_interrupt_handler(interrupt_code);
}

void arch_interrupts_init() {
    kprintf("Interrupt initialisation started...\n");
    for (int i = 0; i < 21; i++) {
        idt[i].offset_1 = (uint16_t) handlers_array[i];
        idt[i].offset_2 = (uint16_t) (handlers_array[i] >> 16);
        idt[i].zero = 0;
        idt[i].selector = system_code_segment;
        idt[i].type = BIT_PRESENT | BIT_LEVEL_0 | BIT_INT_GATE_32;
    }
    for (int i = 21; i < 32; i++) {
        memset(idt + i, 0, sizeof(struct IDT_entry));
    }
    for (int i = 32; i < 48; i++) {
        idt[i].offset_1 = (uint16_t) handlers_array[i - 11];
        idt[i].offset_2 = (uint16_t) (handlers_array[i - 11] >> 16);
        idt[i].zero = 0;
        idt[i].selector = system_code_segment;
        idt[i].type = BIT_PRESENT | BIT_LEVEL_0 | BIT_TRAP_GATE_32;
    }
    load_idt(idt, sizeof(struct IDT_entry) * 48);
    pic_init();
    enable_interrupts();
}