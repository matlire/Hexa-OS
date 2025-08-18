#include "pit.h"

static volatile uint64_t g_ticks;
static uint32_t g_hz;

int pit_probe (void) { return 1; } 

int  pit_init (uint32_t hz)
{
    if (hz == 0) hz = DEFAULT_HZ;
    if (hz > 5000) hz = 5000;
    g_ticks = 0;
    g_hz = hz;

    uint16_t div = (uint16_t)(PIT_BASE_HZ / hz);

    outb(PIT_CMD, PIT_MODE);
    io_wait();
    outb(PIT_CH0, div & 0xFF);
    io_wait();
    outb(PIT_CH0, (div >> 8) & 0xFF);
    io_wait();

    return 1;
}

void pit_enable  (void) {}
void pit_disable (void) {}

uint64_t pit_now_ticks (void) { return g_ticks; }
uint64_t pit_now_ns    (void)
{
    if (!g_hz) return 0;
    return (g_ticks * 1000000000ULL) / g_hz; 
}

void pit_irq_handler (void) { g_ticks++; }

uint32_t pit_get_hz (void) { return g_hz; }
void     pit_set_hz (uint32_t hz) { pit_init(hz); }

const Timer_T PIT_TIMER = {
    .name = "PIT",
    .type = TIMER_PIT,
    .probe = pit_probe,
    .init = pit_init,
    .enable = pit_enable,
    .disable = pit_disable,
    .now_ticks = pit_now_ticks,
    .now_ns = pit_now_ns,
    .irq_handler = pit_irq_handler,
    .get_hz = pit_get_hz,
    .set_hz = pit_set_hz,
    .resolution_ns = 1000000U,
};

