#include "ports.h"

uint8_t inb (uint16_t port)
{
	uint8_t data;
	__asm__ __volatile__("in %%dx, %%al" : "=a" (data) : "Nd" (port));
	return data;
}


void outb (uint16_t port, uint8_t data)
{
	__asm__ __volatile__("out %%al, %%dx" : : "a" (data), "Nd" (port));
}