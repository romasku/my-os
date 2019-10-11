#ifndef MYOS_LOCK_H
#define MYOS_LOCK_H


typedef uint32_t lock_handle;
extern uint32_t lock_is_locked(lock_handle *handle);
extern void lock_acquire(lock_handle *handle);
extern uint32_t lock_acquire_no_block(lock_handle *handle);
extern void lock_release(lock_handle *handle);

#endif //MYOS_LOCK_H
