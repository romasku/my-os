#ifndef OS_ROOT_PORT_IO_H
#define OS_ROOT_PORT_IO_H

extern void io_wait();
extern uint8_t in_byte(uint16_t port);
extern uint16_t in_word(uint16_t port);
extern uint32_t in_dword(uint16_t port);
extern void out_byte(uint16_t port, uint8_t data);
extern void out_word(uint16_t port, uint16_t data);;
extern void out_dword(uint16_t port, uint32_t data);

#endif //OS_ROOT_PORT_IO_H
