#ifndef RPP_595_H
#define RPP_595_H

#include "rpp_gpio.h"

EXPORT_FROM

typedef struct rpp_595 * rpp_595_t;

rpp_595_t rpp_595_create(enum rpp_pin ser, enum rpp_pin srclk, enum rpp_pin clk);

void rpp_595_destroy(rpp_595_t sr);

void rpp_595_set(rpp_595_t sr, u8 clr, u8 set);

void rpp_595_update(rpp_595_t sr);

u8 rpp_595_state(rpp_595_t sr);

void rpp_595_setup(rpp_595_t sr, u8 clr, u8 set);

EXPORT_TO

#endif /* RPP_595_H */
