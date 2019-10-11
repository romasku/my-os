#ifndef MYOS_ARHC_LOCK_H
#define MYOS_ARHC_LOCK_H

#include <stdint.h>

typedef uint32_t lock_handle;

uint32_t lock_is_locked(lock_handle *handle);
uint32_t lock_acquire_no_block(lock_handle *handle);
void lock_acquire_spin(lock_handle *handle);
void lock_acquire_schedule(lock_handle *handle);
void lock_release(lock_handle *handle);


#endif //MYOS_ARHC_LOCK_H
