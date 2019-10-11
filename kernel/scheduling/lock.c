#include <stdint.h>
#include <kernel/scheduling/lock.h>
#include <kernel/arch/lock.h>
#include <kernel/scheduling/tasks.h>

uint32_t lock_is_locked(lock_handle *handle) {
    return handle != 0;
}
uint32_t lock_acquire_no_block(lock_handle *handle) {
    return lock_acquire_no_block_asm(handle);
}
void lock_acquire_spin(lock_handle *handle) {
    while (lock_acquire_no_block_asm(handle) == 0) {
        while (lock_is_locked(handle));
    }
}
void lock_acquire_schedule(lock_handle *handle) {
    while (lock_acquire_no_block_asm(handle) == 0) {
        // It should never go here before scheduling enabled, as there is no concurrency before that.
        // But I am not sure about ISR, so
        // TODO: think more about case when kernel interrupted in critical zone by ISR that also wants to use same zone
        do_scheduling();
    }
}
void lock_release(lock_handle *handle) {
    *handle = 0;
}