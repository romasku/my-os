#ifndef OS_ROOT_INTERRUPTS_H
#define OS_ROOT_INTERRUPTS_H

#include <stdint.h>

#define ERROR_MAX_IRQ_HANDLERS 1
#define ERROR_NO_SUCH_IRQ_HANDLER 2

void arch_interrupts_init();
void interrupts_init();

typedef void (*irq_handler_func)();
void base_interrupt_handler(uint8_t interrupt_code);
int register_irq_handler(int irq, irq_handler_func handler);
int deregister_irq_handler(int irq, irq_handler_func handler);


#endif //OS_ROOT_INTERRUPTS_H
