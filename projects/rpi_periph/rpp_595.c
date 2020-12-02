#include <liber_tools.h>
#include "rpp_595.h"

#define BIT(BYTE, NUM) ((BYTE >> NUM) & 0x01)

#define RPP_595_CLK_SLEEP_MS (1)

struct rpp_595 {
    u8 data;

    enum rpp_pin ser;
    enum rpp_pin clk;
    enum rpp_pin srclk;
};

static void _595_pins_conf(rpp_595_t sr) {
    rpp_gpio_out(sr->ser);
    rpp_gpio_out(sr->clk);
    rpp_gpio_out(sr->srclk);
}

rpp_595_t rpp_595_create(enum rpp_pin ser, enum rpp_pin srclk, enum rpp_pin clk) {
    struct rpp_595 * sr = bxi_malloc(sizeof(struct rpp_595));

    sr->data = 0;
    sr->ser = ser;
    sr->clk = clk;
    sr->srclk = srclk;

    _595_pins_conf(sr);

    return sr;
}

void rpp_595_destroy(rpp_595_t sr) {
    rpp_595_setup(sr, 0x00, 0x00);
    bxi_free(sr);
}

void rpp_595_set(rpp_595_t sr, u8 clr, u8 set) {
    sr->data &= clr;
    sr->data |= set;
}

void rpp_595_setup(rpp_595_t sr, u8 clr, u8 set) {
    rpp_595_set(sr, clr, set);
    rpp_595_update(sr);
}

static void _pin_click(enum rpp_pin pin) {
    rpp_gpio_set(pin, RPP_PIN_ST_HI);
    bxi_msleep(RPP_595_CLK_SLEEP_MS);
    rpp_gpio_set(pin, RPP_PIN_ST_LW);
    bxi_msleep(RPP_595_CLK_SLEEP_MS);
}

void rpp_595_update(rpp_595_t sr) {
    for(u8 i = 0; i < BITS_IN_BYTE; ++i) {
        rpp_gpio_set(sr->ser, BIT(sr->data, i) ? RPP_PIN_ST_HI : RPP_PIN_ST_LW);
        _pin_click(sr->srclk);
    }
    _pin_click(sr->clk);
}

inline u8 rpp_595_state(rpp_595_t sr) {
    return sr->data;
}
