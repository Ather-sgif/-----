#include "bsp_spi.h"

void BSP_SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    /* 开启 GPIO 和 SPI 时钟 */
    RCC_APB2PeriphClockCmd(W25_GPIO_RCC | RCC_APB2Periph_AFIO | RCC_APB2Periph_SPI1, ENABLE);
    RCC_APB2PeriphClockCmd(W25_GPIO_RCC_CS, ENABLE);
    /* CS 引脚配置 */
    GPIO_InitStructure.GPIO_Pin = W25_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(W25_GPIO_PORT_CS, &GPIO_InitStructure);
    W25_CS_HIGH();

    /* SCK & MOSI 配置 */
    GPIO_InitStructure.GPIO_Pin = W25_SCK_PIN | W25_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(W25_GPIO_PORT, &GPIO_InitStructure);

    /* MISO 配置 */
    GPIO_InitStructure.GPIO_Pin = W25_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(W25_GPIO_PORT, &GPIO_InitStructure);

    /* SPI 参数配置 */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;   // W25Q 系列要求 Mode 0
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;    // 软件控制 NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // Fpclk/2
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(W25_SPI, &SPI_InitStructure);

    /* 启动 SPI */
    SPI_Cmd(W25_SPI, ENABLE);
}

uint8_t BSP_SPI_ReadWrite(uint8_t data)
{
    /* 等待发送缓冲区为空 */
    while (SPI_I2S_GetFlagStatus(W25_SPI, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(W25_SPI, data);

    /* 等待接收缓冲区非空 */
    while (SPI_I2S_GetFlagStatus(W25_SPI, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(W25_SPI);
}
