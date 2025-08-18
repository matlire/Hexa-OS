#ifndef PIT_H
#define PIT_H

#include <stdint.h>

#include "time_core.h"
#include "ports.h"

typedef enum
{
    PIT_BASE_HZ = 1193182U, 
    
    PIT_CH0  = 0x40,
    PIT_CMD  = 0x43,
    PIT_MODE = 0x36,
} PIT_Driver;

extern const Timer_T PIT_TIMER;

#endif
