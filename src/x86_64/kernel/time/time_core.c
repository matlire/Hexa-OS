#include "time_core.h"
#include "pit.h"

static const Timer_T *g_current_timer = NULL;
static const Timer_T *g_timers[] = {
    &PIT_TIMER,
};

static inline void cpu_halt (void)
{
    asm volatile("hlt" ::: "memory");
}

void time_core_init_auto (void)
{
    for (int i = 0; i < (sizeof(g_timers) / sizeof(g_timers[0])); i++)
    {
        const Timer_T *curr = g_timers[i];
        if (curr->probe && curr->probe())
        {
            if (curr->init(DEFAULT_HZ))
            {
                g_current_timer = curr;
                if (g_current_timer->enable) g_current_timer->enable();
                return;
            }
        }
    }

    PIT_TIMER.init(DEFAULT_HZ);
    g_current_timer = &PIT_TIMER;
    if (g_current_timer->enable) g_current_timer->enable();
}

void time_core_select_timer (Timer_Types type, uint32_t hz)
{
    if (g_current_timer && g_current_timer->disable) g_current_timer->disable();

    for (int i = 0; i < (sizeof(g_timers) / sizeof(g_timers[0])); i++)
    {
        if (g_timers[i]->type == type)
        {
            const Timer_T *curr = g_timers[i];
            if (!curr->probe || curr->probe())
            {
                if (curr->init(hz))
                {
                    g_current_timer = curr;
                    if (g_current_timer->enable) g_current_timer->enable();
                    return;
                }
            }
        }
    }

    time_core_init_auto();
}

void time_core_irq (void)
{
    if (g_current_timer && g_current_timer->irq_handler) g_current_timer->irq_handler();
}

void sleep_ns (uint64_t ns)
{
    if (!g_current_timer || !g_current_timer->now_ns) return;

    uint64_t start = g_current_timer->now_ns();
    while (time_core_now_ns() - start < ns)
    {
        cpu_halt();
    }
}

void sleep_ms (uint64_t ms)
{
    if (g_current_timer && (g_current_timer->now_ns || g_current_timer->now_ticks)) sleep_ns(ms * 1000000ULL);
}

uint64_t time_core_now_ticks (void)
{
    if (g_current_timer && g_current_timer->now_ticks) return g_current_timer->now_ticks();
    return 0;
}

uint64_t time_core_now_ns (void)
{
    if (!g_current_timer || !g_current_timer->now_ticks || !g_current_timer->get_hz) return 0;
    if (g_current_timer->now_ns) return g_current_timer->now_ns();

    uint64_t ticks = time_core_now_ticks();
    uint32_t hz = time_core_get_hz();
    return ((ticks * 1000000000) / hz);
}

uint64_t time_core_now_ms (void)
{
    if (g_current_timer && (g_current_timer->now_ns || g_current_timer->now_ticks))
    {
        return (time_core_now_ns() / 1000000ULL);
    }
    return 0;
}

uint32_t time_core_get_hz (void)
{
    if (g_current_timer && g_current_timer->get_hz) return g_current_timer->get_hz();
    return 0;
}

Timer_Types time_core_timer_type (void)
{
   return g_current_timer->type; 
}

