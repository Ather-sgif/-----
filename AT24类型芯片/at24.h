#ifndef __AT24_H
#define __AT24_H

#include <stdint.h>
#include <stddef.h>
#include "soft_i2c.h"
#include "system.h"

// EEPROM 配置结构体
typedef struct {
    uint16_t page_size;     // 页大小
    uint32_t size_bytes;    // EEPROM 总容量 (字节)
    uint8_t  addr_bytes;    // 地址字节数 (1 或 2)
    uint8_t  dev_addr;      // 器件地址 (0xA0)
    uint16_t log_start;     // 日志起始地址 (前 log_start 字节保留)
} AT24_Config_t;

// 初始化
void AT24_Init(const AT24_Config_t* cfg);

// 基本操作
int AT24_WriteByte(uint16_t memAddr, uint8_t data);
int AT24_ReadByte(uint16_t memAddr, uint8_t* data);
int AT24_ReadBuffer(uint16_t memAddr, uint8_t* buf, size_t len);
int AT24_WriteBuffer(uint16_t memAddr, const uint8_t* buf, size_t len);

// 日志系统
int AT24_HasData(void);
int AT24_LogWriteByte(uint8_t data);
int AT24_LogWriteBuffer(const uint8_t* buf, size_t len);
int AT24_LogReadAll(uint8_t* buf, size_t max_len, size_t* out_len);
int AT24_LogReadRange(uint16_t offset, uint8_t* buf, size_t len);
int AT24_LogReadReversePage(uint8_t* buf, size_t page_size, uint16_t page_index, size_t* out_len);
#endif
