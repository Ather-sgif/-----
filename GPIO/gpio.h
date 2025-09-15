#ifndef GENERIC_GPIO_H
#define GENERIC_GPIO_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* gpio_port_t; /* 端口句柄，STM32 为 GPIO_TypeDef* */
typedef uint16_t gpio_pin_t; /* 引脚编号，使用 GPIO_Pin_x */

typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT_PP,
    GPIO_MODE_OUTPUT_OD,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG,
} gpio_mode_t;

typedef enum {
    GPIO_NOPULL = 0,
    GPIO_PULLUP,
    GPIO_PULLDOWN,
} gpio_pull_t;

typedef struct {
    const char *name;
    void (*init)(void);
    void (*pin_mode)(gpio_port_t port, gpio_pin_t pin, gpio_mode_t mode, gpio_pull_t pull);
    void (*write)(gpio_port_t port, gpio_pin_t pin, bool value);
    bool (*read)(gpio_port_t port, gpio_pin_t pin);
    void (*toggle)(gpio_port_t port, gpio_pin_t pin);
} gpio_driver_t;

void gpio_register_driver(const gpio_driver_t *drv);
void gpio_init(void);
void gpio_pin_mode(gpio_port_t port, gpio_pin_t pin, gpio_mode_t mode, gpio_pull_t pull);
void gpio_write(gpio_port_t port, gpio_pin_t pin, bool value);
bool gpio_read(gpio_port_t port, gpio_pin_t pin);
void gpio_toggle(gpio_port_t port, gpio_pin_t pin);

#ifdef __cplusplus
}
#endif

#endif