#ifndef OS_ROOT_I8042_H
#define OS_ROOT_I8042_H

#include <stdint.h>
#include <stddef.h>

#define I8042_DEV_1 0x1
#define I8042_DEV_2 0x2

void init_i8042();
uint8_t ps2_get_availability_byte();
void enable_device(uint8_t device_num);
int8_t ps2_device_write(uint8_t device_num, uint8_t byte);
size_t ps2_device_read(uint8_t *buffer, size_t count);


#endif //OS_ROOT_I8042_H
