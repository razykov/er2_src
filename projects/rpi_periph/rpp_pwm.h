#ifndef RPP_PWM_H
#define RPP_PWM_H

#include "rpp_gpio.h"

EXPORT_FROM

typedef struct rpp_pwm * rpp_pwm_t;

rpp_pwm_t rpp_pwm_create(enum rpp_pin pin, u32 freq_hz);

void rpp_pwm_destroy(rpp_pwm_t pwm);

bool rpp_pwm_start(rpp_pwm_t pwm);

bool rpp_pwm_stop(rpp_pwm_t pwm);

void rpp_pwm_update(rpp_pwm_t pwm, u8 value);

EXPORT_TO

#endif /* RPP_PWM_H */
