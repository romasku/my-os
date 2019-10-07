#include <kernel/arch/pci.h>
#include "../util/port_io.h"

#define PCI_ADDR_ENABLED 0x80000000
#define PCI_OFFSET_MASK 0xFC

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

static uint32_t pack_address(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset) {
    uint32_t address = 0;
    address |= PCI_ADDR_ENABLED;
    address |= bus << 16;
    address |= dev << 11;
    address |= func << 8;
    address |= offset & PCI_OFFSET_MASK;
    return address;
}

uint32_t pci_read_dword(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset) {
    uint32_t address = pack_address(bus, dev, func, offset);
    out_dword(PCI_CONFIG_ADDRESS, address);
    return in_dword(PCI_CONFIG_DATA);
}

void pci_write_dword(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset, uint32_t data) {
    uint32_t address = pack_address(bus, dev, func, offset);
    out_dword(PCI_CONFIG_ADDRESS, address);
    out_dword(PCI_CONFIG_DATA, data);
}