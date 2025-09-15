#ifndef __TIMEOUT_H
#define __TIMEOUT_H
#include "system.h"
/* 系统 tick 的时间分辨率（通常 1ms） */
#ifndef TIME_TICK_MS
#define TIME_TICK_MS 1U
#endif

typedef struct {
    uint32_t start_tick;      /* 起始 tick */
    uint32_t timeout_ticks;   /* 超时时间（tick） */
    bool running;
    bool periodic;            /* true = 周期模式，false = 一次性 */
    void (*timeout_cb)(void); /* 可选回调 */
} TimeoutTimer_t;

/* 在 SysTick_Handler() 中调用 */
void TimeTick_Handler(void);

/* 获取当前系统 tick */
uint32_t GetSystemTick(void);

/* 初始化 */
void TimerInit(TimeoutTimer_t* timer, uint32_t timeoutMs, bool periodic);

/* 启停控制 */
void TimerStart(TimeoutTimer_t* timer);
void TimerStop(TimeoutTimer_t* timer);
void TimerReset(TimeoutTimer_t* timer);

/* 检查并处理超时（主循环中调用）
   - 返回 true 表示本次检测到了超时
   - 若有回调，会在此函数内调用 */
bool TimerCheckAndHandle(TimeoutTimer_t* timer);

/* 获取剩余/已用时间（ms） */
uint32_t TimerGetRemainingMs(TimeoutTimer_t* timer);
uint32_t TimerGetElapsedMs(TimeoutTimer_t* timer);

/* 设置回调 */
void TimerSetCallback(TimeoutTimer_t* timer, void (*cb)(void));
#endif /* __TIMEOUT_H */
