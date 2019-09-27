#include <kernel/tty.h>
#include <kernel/arch/tty.h>
#include <kernel/panic.h>
#include <kernel/kprintf.h>

// TODO: do not assume buffer_width == screen_width and same for height

#define buffer_width 80
#define buffer_height 24
char tty_buffer[buffer_height][buffer_width];
void tty_clear_buffer();
void tty_sync_buffer();

size_t screen_width, screen_height;
size_t cursor_x, cursor_y;


void tty_initialize() {
    tty_enable_mode(MODE_80x24);
    tty_clear_screen();
    kprintf("Terminal initialized!\n");
    kprintf("Current mode is %ux%u\n", screen_width, screen_height);
}

void tty_enable_mode(size_t mode) {
    if (mode == MODE_80x24) {
        screen_width = 80;
        screen_height = 24;
        arch_tty_enable_mode(mode);
    } else {
        panic_no_tty();
    }
}

void tty_clear_screen() {
    tty_clear_buffer();
    cursor_x = cursor_y = 0;
    tty_sync_buffer();
}

void tty_clear_buffer() {
    for (size_t y = 0; y < buffer_height; y++) {
        for (size_t x = 0; x < buffer_width; x++) {
            tty_buffer[y][x] = ' ';
        }
    }
}

void tty_sync_buffer() {
    for (size_t y = 0; y < buffer_height; y++) {
        for (size_t x = 0; x < buffer_width; x++) {
            arch_tty_putchar(tty_buffer[y][x], x, y);
        }
    }
}

void _tty_shift_buffer() {
    for (size_t y = 0; y < buffer_height - 1; y++) {
        for (size_t x = 0; x < buffer_width; x++) {
            tty_buffer[y][x] = tty_buffer[y + 1][x];
        }
    }
    for (size_t x = 0; x < buffer_width; x++) {
        tty_buffer[buffer_height - 1][x] = 0;
    }
}

void _tty_putchar_no_sync(char ascii) {
    if (cursor_x == buffer_width || ascii == '\n') {
        cursor_y++;
        cursor_x = 0;
        // Check if we need to scroll text
        if (cursor_y == buffer_height) {
            _tty_shift_buffer();
            cursor_y--;
        }
    }
    if (ascii != '\n') {
        tty_buffer[cursor_y][cursor_x] = ascii;
        cursor_x++;
    }
}

void tty_putchar(char ascii) {
    _tty_putchar_no_sync(ascii);
    tty_sync_buffer();
}

void tty_write(const char *data, size_t size) {
    for (size_t i = 0; i < size ; i++) {
        _tty_putchar_no_sync(data[i]);
    }
    tty_sync_buffer();
}