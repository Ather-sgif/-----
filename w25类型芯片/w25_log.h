#ifndef __W25_LOG_H
#define __W25_LOG_H

#include "w25qxx.h"

typedef struct {
    uint32_t write_addr;
} W25_Log_t;

void W25_LogInit(W25_Log_t *log, uint32_t start_addr);
bool W25_LogWrite(W25_Log_t *log, const uint8_t *data, uint32_t len);
uint16_t W25_LogReadForward(W25_Log_t *log, uint32_t start_addr, uint8_t *buf, uint16_t max_len);
uint16_t W25_LogReadBackward(W25_Log_t *log, uint32_t end_addr, uint8_t *buf, uint16_t max_len);

#endif
