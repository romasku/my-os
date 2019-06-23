#include <stddef.h>
#include <stdint.h>
#include <kernel/tty.h>
#include "vga.h"

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

#define KERNEL_MEM_OFFSET 0xc0000000

void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t *) (0xB8000 + KERNEL_MEM_OFFSET);
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
    terminal_write("Terminal output initialized\n", 28);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll_line() {
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index_cur_line = y * VGA_WIDTH + x;
            const size_t index_next_line = (y + 1) * VGA_WIDTH + x;

            terminal_buffer[index_cur_line] = terminal_buffer[index_next_line];
        }
    }
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_write(const char *data, size_t size) {
    for (size_t i = 0; i < size; i++)
        if (data[i] == '\t') {
            while (terminal_column % 4 != 0) {
                terminal_column++;
            }
            if (terminal_column == VGA_WIDTH) {
                terminal_column = 0;
                terminal_row++;
            }
            if (terminal_row == VGA_HEIGHT) {
                terminal_scroll_line();
                terminal_row--;
            }
        } else if (data[i] == '\n') {
            terminal_row++;
            if (terminal_row == VGA_HEIGHT) {
                terminal_scroll_line();
                terminal_row--;
            }
            terminal_column = 0;
        } else {
            terminal_putentryat(data[i], terminal_color, terminal_column, terminal_row);
            if (++terminal_column == VGA_WIDTH) {
                terminal_column = 0;
                if (++terminal_row == VGA_HEIGHT) {
                    terminal_scroll_line();
                    terminal_row--;
                }
            }
        }
}

