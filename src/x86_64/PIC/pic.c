#include "pic.h"

void PIC_enable (void)
{
	outb(PIC1,		ICW_DEF_INIT);
    outb(PIC2, 		ICW_DEF_INIT);
    outb(PIC1_DATA, PIC_EOI);
    outb(PIC2_DATA, PIC_SLAVE_OFFSET);
    outb(PIC1_DATA, ICW1_INTERVAL4);
    outb(PIC2_DATA, ICW1_SINGLE);
    outb(PIC1_DATA, ICW1_ICW4);
    outb(PIC2_DATA, ICW1_ICW4);
    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);
}

void PIC_sendEOI (uint8_t irq)
{
	if(irq >= 8)
	{
		outb(PIC2_COMMAND, PIC_EOI);
	}
	
	outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_remap (int offset1, int offset2)
{
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);
	io_wait();
	outb(PIC2_DATA, offset2);
	io_wait();
	outb(PIC1_DATA, 4);
	io_wait();
	outb(PIC2_DATA, 2);
	io_wait();
	
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	outb(PIC1_DATA, 0);
	outb(PIC2_DATA, 0);
}

void pic_disable (void)
{
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

void IRQ_set_mask (uint8_t irq)
{
    uint16_t port;
    uint8_t  value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) | (1 << irq);
    outb(port, value);        
}

void IRQ_clear_mask (uint8_t irq)
{
    uint16_t port;
    uint8_t  value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);        
}
