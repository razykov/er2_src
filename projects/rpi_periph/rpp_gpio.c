#include <liber_tools.h>
#include "rpp_gpio.h"
#include "rpp_base.h"

static u8 gpio2gpfsel[] = {
    0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3,3,3,
    4,4,4,4,4,4,4,4,4,4,
    5,5,5,5,5,5,5,5,5,5,
};

static u8 gpio2shift[] = {
    0,3,6,9,12,15,18,21,24,27,
    0,3,6,9,12,15,18,21,24,27,
    0,3,6,9,12,15,18,21,24,27,
    0,3,6,9,12,15,18,21,24,27,
    0,3,6,9,12,15,18,21,24,27,
};

void rpp_gpio_set(enum rpp_pin pin, enum rpp_pin_st state) {
    *(gpio + state) = 1 << (pin & 0x1F);
}

void rpp_gpio_out(enum rpp_pin pin) {
    *(gpio + gpio2gpfsel[pin]) = (*(gpio + gpio2gpfsel[pin]) &
                                   ~(7 << gpio2shift[pin])) |
                                    (1 << gpio2shift[pin]);
}
