#ifndef __DMA_BUFFER_H
#define __DMA_BUFFER_H
#include "system.h"
#define DMA_BUFFER_SIZE 256
#define DMA_BUFFER_COUNT 10
typedef struct
{
    uint8_t data[DMA_BUFFER_COUNT][DMA_BUFFER_SIZE]; //数据缓冲区
    uint16_t write_index;                             //写索引
    uint16_t read_index;                              //读索引
    uint16_t count[DMA_BUFFER_COUNT];                //缓冲区数量

} DmaBuffer_t;
uint16_t dma_buffer_read_count(DmaBuffer_t *buffer);
void dma_buffer_write_count(DmaBuffer_t *buffer, uint16_t count);
void dma_buffer_write_next(DmaBuffer_t *buffer);
void dma_buffer_read_next(DmaBuffer_t *buffer);
bool dma_buffer_is_empty(DmaBuffer_t *buffer);
bool dma_buffer_is_full(DmaBuffer_t *buffer);
#endif // !__DMA_BUFFER_H
