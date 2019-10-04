#include <stdint.h>
#include <kernel/int/interrupts.h>
#include <stddef.h>
#include <kernel/dev/i8042.h>
#include <kernel/kprintf.h>
#include <kernel/config.h>

#define KEYBOARD_IRQ 1
#define KEYBOARD_BUFFER_SIZE 4096

static uint8_t buffer[KEYBOARD_BUFFER_SIZE];
static size_t buffer_count = 0;

void ps2_kbd_irq_handler() {
    uint8_t scancode_data[10];
    size_t count = ps2_device_read(scancode_data, 10);
#if ECHO_SCANCODES == 1
    kprintf("Scan code data: ");
    for (size_t i = 0; i < count; i++) {
        kprintf("%d, ", scancode_data[i]);
    }
    kprintf("\n");
#endif
}

void init_ps2_keyboard() {
    register_irq_handler(KEYBOARD_IRQ, ps2_kbd_irq_handler);
    ps2_enable_device(I8042_DEV_1);
}

void ps2_keyboard_read(uint8_t *buffer, size_t count) {
}