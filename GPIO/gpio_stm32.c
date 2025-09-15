#include "gpio.h"
#include "stm32f10x.h" /* 根据具体 STM32 系列选择合适的头文件 */
#include "stm32f10x_gpio.h"

/* 转换通用 enum 到 STM32 标准库参数 */
static void stm32_pin_mode(gpio_port_t port, gpio_pin_t pin, gpio_mode_t mode, gpio_pull_t pull) {
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef *)port;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = pin;

    /* 根据 mode 设置 GPIO_InitStructure.GPIO_Mode */
    if (mode == GPIO_MODE_INPUT) {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; /* 默认浮空输入 */
        if (pull == GPIO_PULLUP) {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        } else if (pull == GPIO_PULLDOWN) {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
        }
    } else if (mode == GPIO_MODE_OUTPUT_PP) {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    } else if (mode == GPIO_MODE_OUTPUT_OD) {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    } else if (mode == GPIO_MODE_AF) {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; /* 或 GPIO_Mode_AF_OD */
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    } else {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; /* 模拟输入 */
    }

    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

static void stm32_write(gpio_port_t port, gpio_pin_t pin, bool value) {
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef *)port;
    if (value) {
        GPIO_SetBits(GPIOx, pin);
    } else {
        GPIO_ResetBits(GPIOx, pin);
    }
}

static bool stm32_read(gpio_port_t port, gpio_pin_t pin) {
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef *)port;
    return GPIO_ReadInputDataBit(GPIOx, pin) == Bit_SET;
}

static void stm32_toggle(gpio_port_t port, gpio_pin_t pin) {
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef *)port;
    if (GPIO_ReadOutputDataBit(GPIOx, pin) == Bit_SET) {
        GPIO_ResetBits(GPIOx, pin);
    } else {
        GPIO_SetBits(GPIOx, pin);
    }
}

/* 可选：初始化 GPIO 时钟等 */
static void stm32_init(void) {
    /* 示例：使能 GPIOA 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    /* 其他 GPIO 端口类似使能 */
}

const gpio_driver_t gpio_driver_stm32 = {
    .name = "stm32-std",
    .init = stm32_init,
    .pin_mode = stm32_pin_mode,
    .write = stm32_write,
    .read = stm32_read,
    .toggle = stm32_toggle,
};