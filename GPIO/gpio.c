#include "gpio.h"
#include <stddef.h>

static const gpio_driver_t *s_driver = NULL;

void gpio_register_driver(const gpio_driver_t *drv) {
    s_driver = drv;
    /* 注册后立即初始化 */
    if (s_driver && s_driver->init) {
        s_driver->init();
    }
}

void gpio_pin_mode(gpio_port_t port, gpio_pin_t pin, gpio_mode_t mode, gpio_pull_t pull) {
    if (s_driver && s_driver->pin_mode) s_driver->pin_mode(port, pin, mode, pull);
}

void gpio_write(gpio_port_t port, gpio_pin_t pin, bool value) {
    if (s_driver && s_driver->write) s_driver->write(port, pin, value);
}

bool gpio_read(gpio_port_t port, gpio_pin_t pin) {
    if (s_driver && s_driver->read) return s_driver->read(port, pin);
    return false;
}

void gpio_toggle(gpio_port_t port, gpio_pin_t pin) {
    if (s_driver && s_driver->toggle) s_driver->toggle(port, pin);
}