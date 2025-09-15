#ifndef _RINGBUFF_H_INCLUDED
#define _RINGBUFF_H_INCLUDED

#include "system.h"

#define USART_BUFF_MAX 500
#define BUFFER_SIZE 500       /* 环形缓冲区的大小 */

typedef enum {
  CQ_STATUS_OK = 0,
  CQ_STATUS_IS_FULL,
  CQ_STATUS_IS_EMPTY,
  CQ_STATUS_ERR    // 出错
} te_cicrleQueueStatus_t;


typedef struct
{
	uint32_t Head;
	uint32_t Tail;
	uint32_t Length;
	uint8_t Ring_Buff[BUFFER_SIZE];
  uint8_t receive_flag;
} RingBuff_t;


te_cicrleQueueStatus_t RingBuff_IsEmpty(RingBuff_t *rb);

te_cicrleQueueStatus_t RingBuff_IsFull(RingBuff_t *rb);

te_cicrleQueueStatus_t RingBuff_ReadNByte(RingBuff_t *pRingBuff, uint8_t *pData, int size);

te_cicrleQueueStatus_t RingBuff_WriteNByte(RingBuff_t *pRingBuff, uint8_t *pData, int size);

int RingBuff_GetLen(RingBuff_t *pRingBuff);

unsigned char RingBuff_GetIndexItem(RingBuff_t *pRingBuff, int index);

uint8_t Write_RingBuff(RingBuff_t *ringBuff , volatile uint8_t data);
uint8_t Read_RingBuff_Byte(RingBuff_t *ringBuff , uint8_t *rData);
void RingBuff_Init(RingBuff_t *rb);

uint16_t RQBuff_GetBuffLenth(RingBuff_t* RQ_Buff);
bool Ringbuff_IsReceive(RingBuff_t* ringBuff);
void Ringbuff_WriteReceiveFlag(RingBuff_t* ringBuff, bool flag);
#endif //_MOD_BUTTON_H_INCLUDED

