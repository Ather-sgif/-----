#include "at24.h"
#include "semphr.h"

static AT24_Config_t eeprom_cfg;
static SemaphoreHandle_t i2c_mutex;

#define AT24_PTR_ADDR   (eeprom_cfg.size_bytes - 2) // 写指针存储
#define AT24_FLAG_ADDR  (eeprom_cfg.size_bytes - 1) // 数据有效标志

void AT24_Init(const AT24_Config_t* cfg) {
    eeprom_cfg = *cfg;
    if (i2c_mutex == NULL) {
        i2c_mutex = xSemaphoreCreateMutex();
    }
}

// ---------- 基本操作 ----------

int AT24_WriteByte(uint16_t memAddr, uint8_t data) {
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    SoftI2C_Start();
    SoftI2C_WriteByte(eeprom_cfg.dev_addr | 0); // 写模式

    if (eeprom_cfg.addr_bytes == 2) {
        SoftI2C_WriteByte((uint8_t)(memAddr >> 8));
    }
    SoftI2C_WriteByte((uint8_t)(memAddr & 0xFF));
    SoftI2C_WriteByte(data);
    SoftI2C_Stop();

    xSemaphoreGive(i2c_mutex);
    vTaskDelay(pdMS_TO_TICKS(5)); // 写入延时
    return 0;
}

int AT24_ReadByte(uint16_t memAddr, uint8_t* data) {
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    SoftI2C_Start();
    SoftI2C_WriteByte(eeprom_cfg.dev_addr | 0); // 写模式
    if (eeprom_cfg.addr_bytes == 2) {
        SoftI2C_WriteByte((uint8_t)(memAddr >> 8));
    }
    SoftI2C_WriteByte((uint8_t)(memAddr & 0xFF));

    SoftI2C_Start();
    SoftI2C_WriteByte(eeprom_cfg.dev_addr | 1); // 读模式
    *data = SoftI2C_ReadByte(0);
    SoftI2C_Stop();

    xSemaphoreGive(i2c_mutex);
    return 0;
}

int AT24_ReadBuffer(uint16_t memAddr, uint8_t* buf, size_t len) {
    if (!buf || len == 0) return -1;

    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    SoftI2C_Start();
    SoftI2C_WriteByte(eeprom_cfg.dev_addr | 0); // 写模式
    if (eeprom_cfg.addr_bytes == 2) {
        SoftI2C_WriteByte((uint8_t)(memAddr >> 8));
    }
    SoftI2C_WriteByte((uint8_t)(memAddr & 0xFF));

    SoftI2C_Start();
    SoftI2C_WriteByte(eeprom_cfg.dev_addr | 1); // 读模式

    for (size_t i = 0; i < len; i++) {
        if (i == (len - 1)) {
            buf[i] = SoftI2C_ReadByte(0); // NACK
        } else {
            buf[i] = SoftI2C_ReadByte(1); // ACK
        }
    }
    SoftI2C_Stop();

    xSemaphoreGive(i2c_mutex);
    return 0;
}
int AT24_WriteBuffer(uint16_t memAddr, const uint8_t* buf, size_t len) {
    size_t written = 0;

    while (written < len) {
        // 当前页内的偏移
        uint16_t page_offset = memAddr % eeprom_cfg.page_size;

        // 当前页还能写多少字节
        size_t page_remain = eeprom_cfg.page_size - page_offset;

        // 本次要写的字节数（不能跨页）
        size_t chunk = (len - written > page_remain) ? page_remain : (len - written);

        // 开始 I2C 通信
        xSemaphoreTake(i2c_mutex, portMAX_DELAY);

        SoftI2C_Start();
        SoftI2C_WriteByte(eeprom_cfg.dev_addr | 0); // 写

        // 地址
        if (eeprom_cfg.addr_bytes == 2) {
            SoftI2C_WriteByte((uint8_t)(memAddr >> 8)); // 高字节
        }
        SoftI2C_WriteByte((uint8_t)(memAddr & 0xFF));  // 低字节

        // 写 chunk 个字节
        for (size_t i = 0; i < chunk; i++) {
            SoftI2C_WriteByte(buf[written + i]);
        }

        SoftI2C_Stop();
        xSemaphoreGive(i2c_mutex);

        // 写入延时
        vTaskDelay(pdMS_TO_TICKS(5));

        // 更新计数器
        written += chunk;
        memAddr += chunk;
    }

    return 0;
}

// ---------- 日志系统 ----------

static uint16_t AT24_GetWritePtr(void) {
    uint8_t ptr = 0;
    AT24_ReadByte(AT24_PTR_ADDR, &ptr);
    return ptr;
}

static void AT24_SaveWritePtr(uint16_t ptr) {
    AT24_WriteByte(AT24_PTR_ADDR, (uint8_t)(ptr & 0xFF));
}

int AT24_HasData(void) {
    uint8_t flag = 0xFF;
    AT24_ReadByte(AT24_FLAG_ADDR, &flag);
    return (flag == 0xAA);
}

int AT24_LogWriteByte(uint8_t data) {
    uint16_t ptr = AT24_GetWritePtr();
    uint16_t addr = eeprom_cfg.log_start + ptr;

    if (addr >= (eeprom_cfg.size_bytes - 2)) {
        ptr = 0;
        addr = eeprom_cfg.log_start;
    }

    AT24_WriteByte(addr, data);

    ptr++;
    if ((eeprom_cfg.log_start + ptr) >= (eeprom_cfg.size_bytes - 2)) {
        ptr = 0;
    }
    AT24_SaveWritePtr(ptr);

    AT24_WriteByte(AT24_FLAG_ADDR, 0xAA); // 标记有效
    return 0;
}

int AT24_LogWriteBuffer(const uint8_t* buf, size_t len) {
    for (size_t i = 0; i < len; i++) {
        AT24_LogWriteByte(buf[i]);
    }
    return 0;
}

int AT24_LogReadAll(uint8_t* buf, size_t max_len, size_t* out_len) {
    if (!AT24_HasData()) {
        *out_len = 0;
        return -1;
    }

    uint16_t log_size = (eeprom_cfg.size_bytes - 2) - eeprom_cfg.log_start;
    if (max_len < log_size) log_size = max_len;

    AT24_ReadBuffer(eeprom_cfg.log_start, buf, log_size);
    *out_len = log_size;
    return 0;
}

int AT24_LogReadRange(uint16_t offset, uint8_t* buf, size_t len) {
    uint16_t base = eeprom_cfg.log_start + offset;
    if (base + len > (eeprom_cfg.size_bytes - 2)) {
        return -1; // 超出日志区
    }
    return AT24_ReadBuffer(base, buf, len);
}
/**
 * 倒读日志分页
 * 
 * @param buf        输出缓冲区
 * @param page_size  每页大小
 * @param page_index 页号（0 = 最新页，1 = 次新页，依次类推）
 * @param out_len    实际读取的字节数
 * @return 0 成功，-1 无数据，-2 超过日志区
 */
int AT24_LogReadReversePage(uint8_t* buf, size_t page_size, uint16_t page_index, size_t* out_len) {
    if (!AT24_HasData()) {
        *out_len = 0;
        return -1;
    }

    uint16_t log_size = (eeprom_cfg.size_bytes - 2) - eeprom_cfg.log_start;
    uint16_t ptr = AT24_GetWritePtr();

    // 起始偏移 = 写指针 - (page_index * page_size)
    int32_t start = (int32_t)ptr - (int32_t)(page_index * page_size);

    if (start <= -((int32_t)log_size)) {
        *out_len = 0;
        return -2; // 超出日志区
    }

    size_t count = 0;
    while (count < page_size) {
        if (start <= 0) {
            start += log_size; // 回绕
        }
        start--;

        uint8_t data;
        AT24_ReadByte(eeprom_cfg.log_start + (uint16_t)start, &data);
        buf[count++] = data;
    }

    *out_len = count;
    return 0;
}


















































































