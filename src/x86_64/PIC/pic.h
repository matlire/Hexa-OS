#ifndef PIC_H
#define PIC_H

#include "ports.h"

typedef enum {
	PIC1		 = 0x20,	  // Master PIC IO address
	PIC2		 = 0xA0,	  // Slave PIC IO address
	PIC1_COMMAND = PIC1,
	PIC1_DATA	 = (PIC1+1),
	PIC2_COMMAND = PIC2,
	PIC2_DATA	 = (PIC2+1),

	ICW_DEF_INIT	 = 0x11, // Init command

	PIC_EOI		     = 0x20, // End of interrupt

	PIC_SLAVE_OFFSET = 0x28, // PIC slave offset

	ICW1_ICW4		 = 0x01, // Indicates that ICW4 will be present
	ICW1_SINGLE		 = 0x02, // Single (cascade) mode
	ICW1_INTERVAL4	 = 0x04, // Call address interval 4 (8)
	ICW1_LEVEL		 = 0x08, // Level triggered (edge) mode
	ICW1_INIT		 = 0x10, // Initialization - required!

	ICW4_8086		 = 0x01, // 8086/88 (MCS-80/85) mode
	ICW4_AUTO		 = 0x02, // Auto (normal) EOI
	ICW4_BUF_SLAVE	 = 0x08, // Buffered mode/slave
	ICW4_BUF_MASTER	 = 0x0C, // Buffered mode/master
	ICW4_SFNM		 = 0x10, // Special fully nested (not)
} PIC_Consts;

void PIC_enable   (void);
void PIC_sendEOI  (uint8_t irq);

void PIC_remap	  (int offset1, int offset2);
void pic_disable  (void);

void IRQ_set_mask   (uint8_t irq);
void IRQ_clear_mask (uint8_t irq);

#endif