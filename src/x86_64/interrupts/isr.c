#include "isr.h"

static void (* const isr_routines[32])() = {
    isr0,  isr1,  isr2,  isr3,  isr4,  isr5,  isr6,  isr7,
    isr8,  isr9,  isr10, isr11, isr12, isr13, isr14, isr15,
    isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23,
    isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
};

static void (* const irq_routines[16])() = {
    irq0,  irq1,  irq2,  irq3, 
    irq4,  irq5,  irq6,  irq7, 
    irq8,  irq9,  irq10, irq11,
    irq12, irq13, irq14, irq15
};

void isr_install (void) {
    for (int i = 0; i < 32; i++) {
        idt_set_gate((uint8_t)i, (void*)isr_routines[i], ISR_FLAGS);
    }
    
    PIC_enable();
    PIC_remap(IRQ0, IRQ0 + 8);
    IRQ_clear_mask(1);

    for (int i = 0; i < 16; i++) {
        idt_set_gate((uint8_t)(IRQ0 + i), (void*)irq_routines[i], ISR_FLAGS);
    }

    idt_set();
}

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void interrupt_handler (Registers_T *registers)
{
    uint8_t vec = (uint8_t)registers->int_no;

    // CPU interrupts
    if (vec < 32)
    {
        terminal_write_str("Got isr interrupt (");
        char buffer[12];
        int2ascii(vec, buffer);
        terminal_write_str(buffer);
        terminal_write_str("): ");
        terminal_write_str(exception_messages[vec]);
        terminal_write_str("\n");
    }
    // IRQ interrupts
    else if (vec > 31 && vec < 48)
    {
        uint8_t irq = vec - IRQ0;

        if (irq == 1) {
            uint8_t sc = inb(0x60);
            queue_input(sc);
        }

        PIC_sendEOI(irq);
    }
    // Others
    else
    {
        terminal_write_str("Got interrupt: not implemented!");
    }
}
