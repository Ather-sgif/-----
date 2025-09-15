#include "DmaBuffer.h" 

/**
 * @description: 读取当前缓冲区数据长度
 * @param {DmaBuffer_t} *buffer
 * @return {*}
 */
uint16_t dma_buffer_read_count(DmaBuffer_t *buffer)
{
    return buffer->count[buffer->read_index];
}
/**
 * @description: 写入当前缓冲区数据长度
 * @param {DmaBuffer_t} *buffer
 * @param {uint16_t} count
 * @return {*}
 */
void dma_buffer_write_count(DmaBuffer_t *buffer, uint16_t count)
{
    buffer->count[buffer->write_index] = count;
}
/**
 * @description: 写入切换下一个缓冲区
 * @param {DmaBuffer_t} *buffer
 * @return {*}
 */
void dma_buffer_write_next(DmaBuffer_t *buffer)
{
    buffer->write_index = (buffer->write_index + 1) % DMA_BUFFER_COUNT;
}
/**
 * @description: 读取切换下一个缓冲区
 * @param {DmaBuffer_t} *buffer
 * @return {*}
 */
void dma_buffer_read_next(DmaBuffer_t *buffer)
{
    memset(buffer->data[buffer->read_index], 0, DMA_BUFFER_SIZE);
    buffer->read_index = (buffer->read_index + 1) % DMA_BUFFER_COUNT;
}
/**
 * @description: 判断是否有数据
 * @param {DmaBuffer_t} *buffer
 * @return {*}
 */
bool dma_buffer_is_empty(DmaBuffer_t *buffer)
{
    return buffer->read_index == buffer->write_index ? true : false;
}
/**
 * @description: 判断缓冲区是否满
 * @param {DmaBuffer_t} *buffer
 * @return {*}
 */
bool dma_buffer_is_full(DmaBuffer_t *buffer)
{
    return (buffer->write_index + 1) % DMA_BUFFER_COUNT == buffer->read_index;
}