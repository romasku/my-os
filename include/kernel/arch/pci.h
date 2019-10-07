#ifndef MYOS_ARCH_PCI_H
#define MYOS_ARCH_PCI_H

#include <stdint.h>

uint32_t pci_read_dword(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset);
void pci_write_dword(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset, uint32_t data);

#endif //MYOS_ARCH_PCI_H
