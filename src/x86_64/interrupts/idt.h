#ifndef IDT
#define IDT

#include <stdint.h>

// Gate struct
typedef struct
{
	uint16_t isr_low;
	uint16_t kernel_cs;
	uint8_t  ist;
	uint8_t  attrs;
	uint16_t isr_middle;
	uint32_t isr_high;
	uint32_t reserved;
} __attribute__((packed)) IDT_Entry_T;

// idt register struct
typedef struct
{
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) IDTR_T;

#define IDT_ENTRIES 256

void idt_set      (void);
void idt_set_gate (uint8_t vector, void* isr, uint8_t flags);

#endif