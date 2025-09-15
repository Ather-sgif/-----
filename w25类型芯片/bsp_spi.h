#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "system.h"   // 使用标准外设库

/* 使用 SPI1 (PA5-SCK, PA6-MISO, PA7-MOSI, PA4-CS) */
#define W25_SPI             SPI1
#define W25_SPI_RCC         RCC_APB2Periph_SPI1

#define W25_GPIO_PORT       GPIOA
#define W25_GPIO_RCC        RCC_APB2Periph_GPIOA

#define W25_GPIO_PORT_CS    GPIOC
#define W25_GPIO_RCC_CS     RCC_APB2Periph_GPIOC
#define W25_CS_PIN          GPIO_Pin_0

#define W25_SCK_PIN         GPIO_Pin_5
#define W25_MISO_PIN        GPIO_Pin_6
#define W25_MOSI_PIN        GPIO_Pin_7

/* CS 片选控制 */
static inline void W25_CS_LOW(void)  { GPIO_ResetBits(W25_GPIO_PORT_CS, W25_CS_PIN); }
static inline void W25_CS_HIGH(void) { GPIO_SetBits(W25_GPIO_PORT_CS, W25_CS_PIN);  }

/* 接口函数 */
void BSP_SPI_Init(void);
uint8_t BSP_SPI_ReadWrite(uint8_t data);

#endif
