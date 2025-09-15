#ifndef __W25_FACTORY_H
#define __W25_FACTORY_H

#include "w25_table.h"


typedef struct {
    uint32_t jedec_id;
    const char *name;
    uint32_t page_size;
    uint32_t sector_size;
    uint32_t block_size;
    uint32_t capacity;
} W25_Info_t;

W25_Info_t* W25_FactoryCreate(uint32_t jedec_id);

#endif
