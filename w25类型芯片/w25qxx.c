#include "w25qxx.h"

/* 全局芯片信息 */
static W25_Info_t g_w25_info;

/* 内部函数：写使能 */
static void W25_WriteEnable(void)
{
    W25_CS_LOW();
    BSP_SPI_ReadWrite(0x06); // W25_CMD_WRITE_ENABLE
    W25_CS_HIGH();
}

/* 内部函数：读状态 */
static uint8_t W25_ReadStatus(void)
{
    uint8_t s;
    W25_CS_LOW();
    BSP_SPI_ReadWrite(0x05); // W25_CMD_READ_STATUS1
    s = BSP_SPI_ReadWrite(0xFF);
    W25_CS_HIGH();
    return s;
}

/* 内部函数：等待忙 */
static void W25_WaitBusy(void)
{
    while (W25_ReadStatus() & 0x01);
}

/* JEDEC ID 读取 */
static uint32_t W25_ReadID(void)
{
    uint32_t id = 0;
    W25_CS_LOW();
    BSP_SPI_ReadWrite(0x9F);
    id |= BSP_SPI_ReadWrite(0xFF) << 16;
    id |= BSP_SPI_ReadWrite(0xFF) << 8;
    id |= BSP_SPI_ReadWrite(0xFF);
    W25_CS_HIGH();
    return id;
}

/* 判断区域是否全 0xFF */
static bool W25_CheckBlank(uint32_t addr, uint32_t len)
{
    uint8_t buf[32];
    while (len > 0) {
        uint32_t chunk = (len > sizeof(buf)) ? sizeof(buf) : len;
        W25_Read(addr, buf, chunk);
        for (uint32_t i = 0; i < chunk; i++) {
            if (buf[i] != 0xFF) return false;
        }
        addr += chunk;
        len  -= chunk;
    }
    return true;
}

/* 内部 PageProgram */
static void W25_PageProgram(uint32_t addr, const uint8_t *buf, uint16_t len)
{
    W25_WriteEnable();
    W25_CS_LOW();
    BSP_SPI_ReadWrite(0x02); // W25_CMD_PAGE_PROGRAM
    BSP_SPI_ReadWrite((addr >> 16) & 0xFF);
    BSP_SPI_ReadWrite((addr >> 8) & 0xFF);
    BSP_SPI_ReadWrite(addr & 0xFF);
    for (uint16_t i = 0; i < len; i++) {
        BSP_SPI_ReadWrite(buf[i]);
    }
    W25_CS_HIGH();
    W25_WaitBusy();
}

/* API 实现 */
void W25_Init(void)
{
    BSP_SPI_Init();
    uint32_t id = W25_ReadID();

    W25_Info_t *info = W25_FactoryCreate(id);
    if (info) {
        g_w25_info = *info;
    } else {
        g_w25_info.jedec_id = id;
        g_w25_info.name = "Unknown";
        g_w25_info.page_size = 256;
        g_w25_info.sector_size = 4096;
        g_w25_info.block_size = 65536;
        g_w25_info.capacity = 0;
    }
}

W25_Info_t* W25_GetInfo(void)
{
    return &g_w25_info;
}

void W25_Read(uint32_t addr, uint8_t *buf, uint16_t len)
{
    W25_CS_LOW();
    BSP_SPI_ReadWrite(0x03); // W25_CMD_READ_DATA
    BSP_SPI_ReadWrite((addr >> 16) & 0xFF);
    BSP_SPI_ReadWrite((addr >> 8) & 0xFF);
    BSP_SPI_ReadWrite(addr & 0xFF);
    for (uint16_t i = 0; i < len; i++) {
        buf[i] = BSP_SPI_ReadWrite(0xFF);
    }
    W25_CS_HIGH();
}

/* 跨页写入 + 检查是否擦除 */
bool W25_Write(uint32_t addr, const uint8_t *buf, uint32_t len)
{
    uint32_t page_size = g_w25_info.page_size;

    if (!W25_CheckBlank(addr, len)) {
        return false; // 目标区域非空
    }

    while (len > 0) {
        uint32_t page_off   = addr % page_size;
        uint32_t remain_in_page = page_size - page_off;
        uint32_t chunk = (len < remain_in_page) ? len : remain_in_page;

        W25_PageProgram(addr, buf, chunk);

        addr += chunk;
        buf  += chunk;
        len  -= chunk;
    }
    return true;
}

void W25_EraseSector(uint32_t addr)
{
    W25_WriteEnable();
    W25_CS_LOW();
    BSP_SPI_ReadWrite(0x20); // W25_CMD_SECTOR_ERASE
    BSP_SPI_ReadWrite((addr >> 16) & 0xFF);
    BSP_SPI_ReadWrite((addr >> 8) & 0xFF);
    BSP_SPI_ReadWrite(addr & 0xFF);
    W25_CS_HIGH();
    W25_WaitBusy();
}
