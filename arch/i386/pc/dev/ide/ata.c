#include <kernel/arch/ide.h>
#include <kernel/panic.h>
#include "../../util/port_io.h"

#define IO_PORTS_COUNT 8
#define CONTROL_PORTS_COUNT 2

struct bus_data {
    uint16_t io_base_port;
    uint16_t control_base_port;
};
static struct bus_data ide_buses[2] = {
        {0x1F0, 0x3F6},
        {0x170, 0x376}
};

static void check_bus(uint8_t bus) {
    if (bus >= sizeof(ide_buses) / sizeof(struct bus_data)) {
        panic("IDE: trying to use not supported bus number");
    }
}

static void check_register_io(uint8_t reg_num) {
    if (reg_num >= IO_PORTS_COUNT) {
        panic("IDE: trying to use not existing io register");
    }
}

static void check_register_control(uint8_t reg_num) {
    if (reg_num >= CONTROL_PORTS_COUNT) {
        panic("IDE: trying to use not existing control register");
    }
}


uint8_t ide_read_io_register_byte(uint8_t bus, uint8_t reg_num) {
    check_bus(bus);
    check_register_io(reg_num);
    return in_byte(ide_buses[bus].io_base_port + reg_num);
}
uint16_t ide_read_io_register_word(uint8_t bus, uint8_t reg_num) {
    check_bus(bus);
    check_register_io(reg_num);
    return in_word(ide_buses[bus].io_base_port + reg_num);
}
void ide_write_io_register_byte(uint8_t bus, uint8_t reg_num, uint8_t data) {
    check_bus(bus);
    check_register_io(reg_num);
    out_byte(ide_buses[bus].io_base_port + reg_num, data);
}
void ide_write_io_register_word(uint8_t bus, uint8_t reg_num, uint16_t data) {
    check_bus(bus);
    check_register_io(reg_num);
    out_word(ide_buses[bus].io_base_port + reg_num, data);
}

uint8_t ide_read_control_register_byte(uint8_t bus, uint8_t reg_num) {
    check_bus(bus);
    check_register_control(reg_num);
    return in_byte(ide_buses[bus].control_base_port + reg_num);
}
void ide_write_control_register_byte(uint8_t bus, uint8_t reg_num, uint8_t data) {
    check_bus(bus);
    check_register_control(reg_num);
    out_byte(ide_buses[bus].control_base_port + reg_num, data);
}