#ifndef MYOS_LOCK_H
#define MYOS_LOCK_H

#include <stdint.h>

extern uint32_t lock_acquire_no_block_asm(uint32_t *handle);


#endif //MYOS_LOCK_H
