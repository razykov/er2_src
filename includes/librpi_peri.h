#ifndef RPI_PERI_H
#define RPI_PERI_H

#include <libbixi.h>


bool rpp_init(void);

void rpp_free(void);


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


typedef struct rpp_pwm * rpp_pwm_t;

rpp_pwm_t rpp_pwm_create(enum rpp_pin pin, u32 freq_hz);

void rpp_pwm_destroy(rpp_pwm_t pwm);

bool rpp_pwm_start(rpp_pwm_t pwm);

bool rpp_pwm_stop(rpp_pwm_t pwm);

void rpp_pwm_update(rpp_pwm_t pwm, u8 value);


typedef struct rpp_595 * rpp_595_t;

rpp_595_t rpp_595_create(enum rpp_pin ser, enum rpp_pin srclk, enum rpp_pin clk);

void rpp_595_destroy(rpp_595_t sr);

void rpp_595_set(rpp_595_t sr, u8 clr, u8 set);

void rpp_595_update(rpp_595_t sr);

u8 rpp_595_state(rpp_595_t sr);

void rpp_595_setup(rpp_595_t sr, u8 clr, u8 set);

#endif /* RPI_PERI_H */
