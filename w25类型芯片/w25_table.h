#ifndef __W25_TABLE_H
#define __W25_TABLE_H

#include <stdint.h>
#include "system.h"
typedef struct {
    uint32_t jedec_id;
    const char *name;
    uint32_t page_size;
    uint32_t sector_size;
    uint32_t block_size;
    uint32_t capacity;
} W25_ChipInfo_t;

static const W25_ChipInfo_t w25_chip_table[] = {
    {0xEF4014, "W25Q80",   256, 4096, 65536,  1*1024*1024},
    {0xEF4015, "W25Q16",   256, 4096, 65536,  2*1024*1024},
    {0xEF4016, "W25Q32",   256, 4096, 65536,  4*1024*1024},
    {0xEF4017, "W25Q64",   256, 4096, 65536,  8*1024*1024},
    {0xEF4018, "W25Q128",  256, 4096, 65536, 16*1024*1024},
    {0xEF4019, "W25Q256",  256, 4096, 65536, 32*1024*1024},
    {0, NULL, 0, 0, 0, 0}
};

#endif
