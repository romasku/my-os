#include <kernel/bus/pci.h>
#include <stdint.h>
#include <kernel/arch/pci.h>
#include <kernel/kprintf.h>
#include "pci.h"

static uint16_t pci_read_byte(uint8_t bus, uint8_t dev, uint8_t fun, uint8_t offset) {
    uint32_t data = pci_read_dword(bus, dev, fun, offset);
    return data >> offset % 4;
}

static uint16_t pci_read_word(uint8_t bus, uint8_t dev, uint8_t fun, uint8_t offset) {
    uint32_t data = pci_read_dword(bus, dev, fun, offset);
    return offset & 0x2 ? data >> 16 : data;
}

static uint8_t is_device_valid(uint8_t bus, uint8_t dev) {
    uint16_t vendor = pci_read_word(bus, dev, 0, 0);
    return vendor != PCI_NO_VENDOR;
}

void init_pci_bus() {
    for (uint16_t bus = 0; bus < 256; bus++) {
        for(uint8_t dev = 0; dev < 32; dev++) {
            if (is_device_valid(bus, dev)) {
                kprintf("PCI DEV: %d.%d - %u\n", bus, dev, pci_read_dword(bus, dev, 0, 0));
            }
        }
    }
}