#include <stdint.h>
#include "pic.h"
#include "../util/port_io.h"

#define PIC1            0x20
#define PIC2            0xA0
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	    (PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	    (PIC2+1)

#define PIC_EOI		    0x20

#define ICW1_ICW4	    0x01		/* Enable 4 command word */
#define ICW1_INIT	    0x10		/* Initialization - required! */

#define ICW4_8086	    0x01		/* 8086/88 (MCS-80/85) mode */


void pic_init() {

    out_byte(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
    out_byte(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    out_byte(PIC1_DATA, 32);      // ICW2: Master PIC vector offset
    io_wait();
    out_byte(PIC2_DATA, 40);      // ICW2: Slave PIC vector offset
    io_wait();
    out_byte(PIC1_DATA, 4);       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    out_byte(PIC2_DATA, 2);       // ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait();

    out_byte(PIC1_DATA, ICW4_8086);
    io_wait();
    out_byte(PIC2_DATA, ICW4_8086);
    io_wait();

}

void pic_end_of_interrupt(uint8_t req) {
    if (req > 7) {
        out_byte(PIC2_COMMAND, PIC_EOI);
    }
    out_byte(PIC1_COMMAND, PIC_EOI);
}
