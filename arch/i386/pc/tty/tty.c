#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/arch/tty.h>
#include <kernel/mm.h>
#include <kernel/panic.h>

#include "vga.h"

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

uint8_t terminal_color;
uint16_t *terminal_buffer;

void arch_tty_enable_mode(size_t mode) {
    if (mode == MODE_80x24) {
        terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
        terminal_buffer = (uint16_t *) (0xB8000 + KERNEL_MEM_OFFSET);
    } else {
        // panic here (we do not have even output, so just block here;
        panic_no_tty();
    }
}

void arch_tty_putchar(char c, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, terminal_color);
}

void arch_tty_move_cursor(size_t x, size_t y) {
    // TODO: implement it
}


