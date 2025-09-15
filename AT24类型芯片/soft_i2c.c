#include "soft_i2c.h"

// 定义IO
#define I2C_SCL_PORT GPIOB
#define I2C_SDA_PORT GPIOB
#define I2C_SCL_PIN  GPIO_Pin_6
#define I2C_SDA_PIN  GPIO_Pin_7

#define SDA_H() GPIO_SetBits(I2C_SDA_PORT, I2C_SDA_PIN)
#define SDA_L() GPIO_ResetBits(I2C_SDA_PORT, I2C_SDA_PIN)
#define SCL_H() GPIO_SetBits(I2C_SCL_PORT, I2C_SCL_PIN)
#define SCL_L() GPIO_ResetBits(I2C_SCL_PORT, I2C_SCL_PIN)

#define SDA_READ() GPIO_ReadInputDataBit(I2C_SDA_PORT, I2C_SDA_PIN)


void SoftI2C_Init(void) {
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;

    // SCL
    gpio.GPIO_Pin = I2C_SCL_PIN;
    gpio.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(I2C_SCL_PORT, &gpio);

    // SDA
    gpio.GPIO_Pin = I2C_SDA_PIN;
    gpio.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(I2C_SDA_PORT, &gpio);

    SCL_H();
    SDA_H();
}

void SoftI2C_Start(void) {
    SDA_H(); SCL_H(); delay_us(5);
    SDA_L(); delay_us(5);
    SCL_L();
}

void SoftI2C_Stop(void) {
    SDA_L(); delay_us(5);
    SCL_H(); delay_us(5);
    SDA_H(); delay_us(5);
}

uint8_t SoftI2C_WriteByte(uint8_t data) {
    for (int i=0;i<8;i++) {
        if (data & 0x80) SDA_H(); else SDA_L();
        delay_us(2);
        SCL_H(); delay_us(5);
        SCL_L(); delay_us(2);
        data <<= 1;
    }
    // ACK
    SDA_H();
    delay_us(2);
    SCL_H();
    uint8_t ack = !SDA_READ();
    delay_us(5);
    SCL_L();
    return ack;
}

uint8_t SoftI2C_ReadByte(uint8_t ack) {
    uint8_t data = 0;
    SDA_H();
    for (int i=0;i<8;i++) {
        data <<= 1;
        SCL_H(); delay_us(5);
        if (SDA_READ()) data |= 1;
        SCL_L(); delay_us(5);
    }
    // ACK/NACK
    if (ack) SDA_L(); else SDA_H();
    delay_us(2);
    SCL_H(); delay_us(5);
    SCL_L();
    SDA_H();
    return data;
}
