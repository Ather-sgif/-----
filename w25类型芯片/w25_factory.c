#include "w25_factory.h"

static W25_Info_t g_w25_info;
W25_Info_t* W25_FactoryCreate(uint32_t jedec_id)
{
    for (int i = 0; w25_chip_table[i].jedec_id != 0; i++) {
        if (w25_chip_table[i].jedec_id == jedec_id) {
            g_w25_info = (W25_Info_t){
                jedec_id,
                w25_chip_table[i].name,
                w25_chip_table[i].page_size,
                w25_chip_table[i].sector_size,
                w25_chip_table[i].block_size,
                w25_chip_table[i].capacity
            };
            return &g_w25_info;
        }
    }

    /* 未知芯片 → 返回 NULL，应用层决定如何处理 */
    return NULL;
}
