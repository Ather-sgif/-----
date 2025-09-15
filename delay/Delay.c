#include "Delay.h"
// 定义CPU时钟频率，用于将时间转换为周期数  
#define CPU_CLOCK_FREQ 72000000UL    
 
// 定义与数据观察与跟踪（DWT）模块相关的寄存器地址和位掩码  
#define DEM_CR             *((volatile uint32_t *)0xE000EDFC) 
#define DWT_CR             *((volatile uint32_t *)0xE0001000) 
#define DWT_CYCCNT         *((volatile uint32_t *)0xE0001004) 
#define DEM_CR_TRCENA      0x01000000   
#define DWT_CR_CYCCNTENA   0x00000001 

void delay_init(void) {
	// 使能 DWT 和 CYCCNT
	DEM_CR |= DEM_CR_TRCENA; // 使能跟踪和调试块
	DWT_CYCCNT = 0;          // 清零周期计数器
	DWT_CR |= DWT_CR_CYCCNTENA; // 使能周期计数器
}
/**
  * @brief  微秒级延时
  * @param  xus 延时时长，范围：0~233015
  * @retval 无
  */
void delay_us(uint32_t xus)
{
	uint32_t startTick = DWT_CYCCNT;
	uint32_t delayTicks = (CPU_CLOCK_FREQ / 1000000) * xus; // 计算需要延时的周期数
	while ((DWT_CYCCNT - startTick) < delayTicks);  

}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void delay_ms(uint32_t xms)
{
	while(xms--)
	{
		delay_us(1000);
	}
}
 
/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void delay_s(uint32_t xs)
{
	while(xs--)
	{
		delay_ms(1000);
	}
} 
