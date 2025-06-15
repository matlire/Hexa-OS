#include <idt.h>

idt_entry_t idt_entry[IDT_ENTRIES];
idtr_t 	   idtr;

// Set idtr register
void idt_set(void)
{
	idtr.base = (uint64_t) &idt_entry;
    idtr.limit = IDT_ENTRIES * sizeof(idt_entry_t) - 1; // Size for idt = 256*16 bytes - 1 = 4095 bytes
    __asm__ __volatile__("lidt (%0)" : : "r" (&idtr));  // Save idtr address into register
}

// Setup interrupt gate with interrupt vector, isr hadler and flags
void idt_set_gate (uint8_t vector, void* isr, uint8_t flags)
{
	idt_entry_t* gate = &idt_entry[vector];

	gate->isr_low     = (uint64_t)isr & 0xFFFF;
	gate->kernel_cs   = 0x08;
	gate->ist 	      = 0;
	gate->attrs       = flags;
	gate->isr_middle  = ((uint64_t)isr >> 16) & 0xFFFF; 
	gate->isr_high    = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
	gate->reserved    = 0;
}