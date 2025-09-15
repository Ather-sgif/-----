#include "TimeOut.h"

volatile uint32_t system_tick = 0;

void TimeTick_Handler(void)
{
    system_tick++;
}

uint32_t GetSystemTick(void)
{
    return system_tick;
}

/* 工具函数：ms -> tick */
static uint32_t ms_to_ticks(uint32_t ms)
{
    if (ms == 0) return 1; /* 不允许 0 */
    return (ms + TIME_TICK_MS - 1) / TIME_TICK_MS;
}

void TimerInit(TimeoutTimer_t* timer, uint32_t timeoutMs, bool periodic)
{
    if (!timer) return;
    timer->timeout_ticks = ms_to_ticks(timeoutMs);
    timer->start_tick = 0;
    timer->running = false;
    timer->periodic = periodic;
    timer->timeout_cb = NULL;
}

void TimerStart(TimeoutTimer_t* timer)
{
    if (!timer) return;
    timer->start_tick = GetSystemTick();
    timer->running = true;
}

void TimerStop(TimeoutTimer_t* timer)
{
    if (!timer) return;
    timer->running = false;
}

void TimerReset(TimeoutTimer_t* timer)
{
    if (!timer) return;
    timer->start_tick = GetSystemTick();
    timer->running = true;
}

bool TimerCheckAndHandle(TimeoutTimer_t* timer)
{
    if (!timer || !timer->running) return false;

    uint32_t now = GetSystemTick();
    uint32_t elapsed = now - timer->start_tick;

    if (elapsed >= timer->timeout_ticks) {
        if (timer->periodic) {
            /* 保持周期对齐，避免漂移 */
            uint32_t periods = elapsed / timer->timeout_ticks;
            timer->start_tick += periods * timer->timeout_ticks;
        } else {
            timer->running = false;
        }

        if (timer->timeout_cb) {
            timer->timeout_cb();
        }
        return true;
    }
    return false;
}

uint32_t TimerGetRemainingMs(TimeoutTimer_t* timer)
{
    if (!timer || !timer->running) return 0;
    uint32_t now = GetSystemTick();
    uint32_t elapsed = now - timer->start_tick;
    if (elapsed >= timer->timeout_ticks) return 0;
    return (timer->timeout_ticks - elapsed) * TIME_TICK_MS;
}

uint32_t TimerGetElapsedMs(TimeoutTimer_t* timer)
{
    if (!timer || !timer->running) return 0;
    uint32_t now = GetSystemTick();
    uint32_t elapsed = now - timer->start_tick;
    return elapsed * TIME_TICK_MS;
}

void TimerSetCallback(TimeoutTimer_t* timer, void (*cb)(void))
{
    if (!timer) return;
    timer->timeout_cb = cb;
}
