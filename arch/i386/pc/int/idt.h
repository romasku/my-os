//
// Created by romasku on 24.11.18.
//

#ifndef MY_OS2_IDT_H
#define MY_OS2_IDT_H

#include <stdint.h>

#define BIT_PRESENT         1 << 7
#define BIT_LEVEL_0         0 << 5
#define BIT_LEVEL_1         1 << 5
#define BIT_LEVEL_2         2 << 5
#define BIT_LEVEL_3         3 << 5
#define BIT_TASK_GATE       0x5
#define BIT_INT_GATE_16     0x6
#define BIT_INT_GATE_32     0xE
#define BIT_TRAP_GATE_16    0x7
#define BIT_TRAP_GATE_32    0xF

struct IDT_entry {
    uint16_t offset_1;
    uint16_t selector;
    uint8_t zero;
    uint8_t type;
    uint16_t offset_2;
} __attribute__((packed));


#endif //MY_OS2_IDT_H
