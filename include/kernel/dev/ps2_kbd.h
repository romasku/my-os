#ifndef MYOS_PS2_KBD_H
#define MYOS_PS2_KBD_H

void init_ps2_keyboard();
void ps2_keyboard_read(uint8_t *buffer, size_t count);

#endif //MYOS_PS2_KBD_H
