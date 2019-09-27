#include <stdint.h>
#include <kernel/dev/timer.h>
#include <kernel/int/interrupts.h>
#include "../util/port_io.h"

#define PIT_PORT_DATA_0 0x40
#define PIT_PORT_COMMAND 0x43

#define PIT_CHANNEL_0 0
#define PIT_ACCESS_LO_HI 0b00110000
#define PIT_MODE_RATE 0b0100
#define PIT_MODE_BINARY 0
#define PIT_FREQ_HZ 1193182

void pit_set_frequency(uint16_t freq_hz) {
    uint16_t divider = PIT_FREQ_HZ / freq_hz;
    out_byte(PIT_PORT_COMMAND, PIT_CHANNEL_0 | PIT_ACCESS_LO_HI | PIT_MODE_RATE | PIT_MODE_BINARY);
    out_byte(PIT_PORT_DATA_0, (uint8_t)divider);
    out_byte(PIT_PORT_DATA_0, (uint8_t)(divider >> 8));
}

void arch_init_timer() {
    pit_set_frequency(CLOCK_FREQ);
}

void arch_register_clock_func(clock_func func) {
    register_irq_handler(0, func);
}

