#ifndef TIME_CORE_H
#define TIME_CORE_H

#include "vga_print.h"

#include <stdint.h>

#define DEFAULT_HZ 1000

typedef enum
{
    TIMER_NONE,
    TIMER_PIT,
    TIMER_APIC,
    TIMER_HPET,
} Timer_Types;

typedef struct
{
    const char *name;
    const Timer_Types type;

    int (*probe) (void);

    int  (*init)    (uint32_t hz);
    void (*enable)  (void);
    void (*disable) (void);

    uint64_t (*now_ticks) (void);
    uint64_t (*now_ns)    (void);

    void (*irq_handler) (void);

    uint32_t (*get_hz) (void);
    void     (*set_hz) (uint32_t hz);

    uint32_t resolution_ns;
} Timer_T;

void time_core_init_auto (void);

void time_core_select_timer (Timer_Types type, uint32_t hz);

void time_core_irq (void);
void sleep_ns       (uint64_t ns);
void sleep_ms       (uint64_t ms);

uint64_t time_core_now_ticks (void);
uint64_t time_core_now_ns    (void);
uint64_t time_core_now_ms    (void);

uint32_t time_core_get_hz (void);

Timer_Types time_core_timer_type (void);

#endif
