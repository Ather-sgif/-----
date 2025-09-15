#include "w25_log.h"

static W25_Info_t *info;

/* 判断扇区是否空 */
static bool W25_SectorBlank(uint32_t addr)
{
    uint8_t buf[32];
    uint32_t size = info->sector_size;

    for (uint32_t offset = 0; offset < size; offset += sizeof(buf)) {
        W25_Read(addr + offset, buf, sizeof(buf));
        for (uint32_t i = 0; i < sizeof(buf); i++) {
            if (buf[i] != 0xFF) return false;
        }
    }
    return true;
}

/* 初始化日志系统，定位写指针 */
void W25_LogInit(W25_Log_t *log, uint32_t start_addr)
{
    info = W25_GetInfo();
    log->write_addr = start_addr;

    /* 扇区扫描（考虑非对齐情况） */
    while (log->write_addr < info->capacity) {
        if (W25_SectorBlank(log->write_addr)) break;
        log->write_addr += info->sector_size;
    }

    /* 精确定位（逐字节） */
    uint8_t tmp;
    while (log->write_addr < info->capacity) {
        W25_Read(log->write_addr, &tmp, 1);
        if (tmp == 0xFF) break;
        log->write_addr++;
    }

}

/* 写日志，自动处理写满 */
bool W25_LogWrite(W25_Log_t *log, const uint8_t *data, uint32_t len)
{
    W25_Info_t *info = W25_GetInfo();

    if (len > info->capacity) {
        return false; // 数据太大
    }

    /* 检查是否写满 */
    if (log->write_addr + len > info->capacity) {
        /* 计算涉及的扇区范围 */
        uint32_t start_sector = 0;
        uint32_t end_sector = (len - 1) / info->sector_size;

        /* 擦除所需扇区 */
        for (uint32_t i = start_sector; i <= end_sector; i++) {
            W25_EraseSector(i * info->sector_size);
        }

        log->write_addr = 0; // 从头覆盖
    }

    if (!W25_Write(log->write_addr, data, len)) {
        return false;
    }

    log->write_addr += len;
    return true;
}

/* 顺序读取 */
uint16_t W25_LogReadForward(W25_Log_t *log, uint32_t start_addr, uint8_t *buf, uint16_t max_len)
{
    if (start_addr >= log->write_addr) return 0;
    uint16_t len = (start_addr + max_len <= log->write_addr) ? max_len : (log->write_addr - start_addr);
    W25_Read(start_addr, buf, len);
    return len;
}
/* 逆序读取 */
uint16_t W25_LogReadBackward(W25_Log_t *log, uint32_t end_addr, uint8_t *buf, uint16_t max_len)
{
    if (end_addr == 0 || end_addr > log->write_addr) {
        return 0; // 无效地址
    }

    uint32_t count = (end_addr >= max_len) ? max_len : end_addr;

    /* 倒序读取 */
    for (uint32_t i = 0; i < count; i++) {
        W25_Read(end_addr - 1 - i, &buf[i], 1);
    }

    return count;
}
