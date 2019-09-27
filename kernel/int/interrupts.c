#include <kernel/int/interrupts.h>
#include <stdint.h>
#include <kernel/kprintf.h>
#include <stddef.h>

#define IRQ_NUM 16
#define MAX_IRQ_HANDLERS 4

irq_handler_func handlers[IRQ_NUM][MAX_IRQ_HANDLERS];

void interrupts_init() {
    arch_interrupts_init();
}

void base_interrupt_handler(uint8_t interrupt_code) {
    if (interrupt_code < 32) {
        kprintf("CPU interrupt happened: %d\n", interrupt_code);
    } else {
        kprintf("IRQ happened: %d\n", interrupt_code - 32);
        int irq = interrupt_code - 32;
        for (int i = 0; i < MAX_IRQ_HANDLERS; i++) {
            if (handlers[irq][i] != NULL) {
                handlers[irq][i]();
            }
        }
    }
}

int register_irq_handler(int irq, irq_handler_func handler) {
    for (int i = 0; i < MAX_IRQ_HANDLERS; i++) {
        if (handlers[irq][i] == NULL) {
            handlers[irq][i] = handler;
            return 0;
        }
    }
    return ERROR_MAX_IRQ_HANDLERS;
}

int deregister_irq_handler(int irq, irq_handler_func handler) {
    for (int i = 0; i < MAX_IRQ_HANDLERS; i++) {
        if (handlers[irq][i] == handler) {
            handlers[irq][i] = NULL;
            return 0;
        }
    }
    return ERROR_NO_SUCH_IRQ_HANDLER;
}