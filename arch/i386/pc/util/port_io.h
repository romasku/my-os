#ifndef OS_ROOT_PORT_IO_H
#define OS_ROOT_PORT_IO_H

extern void io_wait();
extern uint8_t in_byte(uint16_t port);
extern void out_byte(uint16_t port, uint8_t data);

#endif //OS_ROOT_PORT_IO_H
