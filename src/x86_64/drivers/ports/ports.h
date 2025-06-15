#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

uint8_t inb  (uint16_t port);
void    outb (uint16_t port, uint8_t data);

// Port 0x80 is used for logging info, so sending data there takes 1-4 ms and can be used as delay 
static inline void io_wait (void)
{
	outb(0x80, 0);
}

#endif