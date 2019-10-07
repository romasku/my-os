#include <stdint.h>

#ifndef MYOS_ARCH_IDE_H
#define MYOS_ARCH_IDE_H

uint8_t ide_read_io_register_byte(uint8_t bus, uint8_t reg_num);
uint16_t ide_read_io_register_word(uint8_t bus, uint8_t reg_num);
void ide_write_io_register_byte(uint8_t bus, uint8_t reg_num, uint8_t data);
void ide_write_io_register_word(uint8_t bus, uint8_t reg_num, uint16_t data);

uint8_t ide_read_control_register_byte(uint8_t bus, uint8_t reg_num);
void ide_write_control_register_byte(uint8_t bus, uint8_t reg_num, uint8_t data);

#endif //MYOS_ARCH_IDE_H
