#ifndef PIC_H
#define PIC_H

#include "ports.h"

#define PIC1		 0x20	  // Master PIC IO address
#define PIC2		 0xA0	  // Slave PIC IO address
#define PIC1_COMMAND PIC1
#define PIC1_DATA	 (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA	 (PIC2+1)

#define ICW_DEF_INIT	0x11    // Init command

#define PIC_EOI		    0x20    // End of interrupt

#define ICW1_ICW4		0x01	// Indicates that ICW4 will be present
#define ICW1_SINGLE		0x02	// Single (cascade) mode
#define ICW1_INTERVAL4	0x04	// Call address interval 4 (8)
#define ICW1_LEVEL		0x08	// Level triggered (edge) mode
#define ICW1_INIT		0x10	// Initialization - required!

#define ICW4_8086		0x01	// 8086/88 (MCS-80/85) mode
#define ICW4_AUTO		0x02	// Auto (normal) EOI
#define ICW4_BUF_SLAVE	0x08	// Buffered mode/slave
#define ICW4_BUF_MASTER	0x0C	// Buffered mode/master
#define ICW4_SFNM		0x10	// Special fully nested (not)

void PIC_enable   (void);
void PIC_sendEOI  (uint8_t irq);

void PIC_remap	  (int offset1, int offset2);
void pic_disable  (void);

void IRQ_set_mask   (uint8_t irq);
void IRQ_clear_mask (uint8_t irq);

#endif