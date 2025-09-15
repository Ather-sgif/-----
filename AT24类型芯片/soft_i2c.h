#ifndef __SOFT_I2C_H
#define __SOFT_I2C_H
#include"system.h"
// 软件 I2C 初始化
void SoftI2C_Init(void);

// I2C 基本操作
void SoftI2C_Start(void);
void SoftI2C_Stop(void);
uint8_t SoftI2C_WriteByte(uint8_t data);
uint8_t SoftI2C_ReadByte(uint8_t ack);

#endif
