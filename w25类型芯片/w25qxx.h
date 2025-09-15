#ifndef __W25QXX_H
#define __W25QXX_H

#include "bsp_spi.h"
#include "w25_factory.h"

void W25_Init(void);
W25_Info_t* W25_GetInfo(void);

void W25_Read(uint32_t addr, uint8_t *buf, uint16_t len);
bool W25_Write(uint32_t addr, const uint8_t *buf, uint32_t len);
void W25_EraseSector(uint32_t addr);

#endif
