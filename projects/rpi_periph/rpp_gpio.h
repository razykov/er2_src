#ifndef RPP_GPIO_H
#define RPP_GPIO_H

#include <libbixi.h>

EXPORT_FROM

#define GPIOTOGPSET (0x07)
#define GPIOTOGPCLR (0x0a)

enum rpp_pin_st {
    RPP_PIN_ST_LW = GPIOTOGPCLR,
    RPP_PIN_ST_HI = GPIOTOGPSET
};

enum rpp_pin {
    RPP_PIN_LED    = 4,
    RPP_PIN_SER    = 10,
    RPP_PIN_SRCLK  = 11,
    RPP_PIN_CLK    = 22,
    RPP_PIN_PWM_LB = 18,
    RPP_PIN_PWM_RB = 23,
    RPP_PIN_PWM_LF = 24,
    RPP_PIN_PWM_RF = 25,
};

void rpp_gpio_set(enum rpp_pin pin, enum rpp_pin_st state);

void rpp_gpio_out(enum rpp_pin pin);

EXPORT_TO

#endif /* RPP_GPIO_H */
