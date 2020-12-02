#include <liber_tools.h>
#include <librpi_peri.h>
#include "er2_wheels.h"

#define WHEELS_FREQ_HZ (40)

enum er2_wheel_off {
    ER2_WHL_RF = 2 * ER2_WHL_RFI,
    ER2_WHL_LF = 2 * ER2_WHL_LFI,
    ER2_WHL_RB = 2 * ER2_WHL_RBI,
    ER2_WHL_LB = 2 * ER2_WHL_LBI
};

enum er2_wheel_clr {
    ER2_WHL_RF_CLR = ~(0b11 << ER2_WHL_RF),
    ER2_WHL_LF_CLR = ~(0b11 << ER2_WHL_LF),
    ER2_WHL_RB_CLR = ~(0b11 << ER2_WHL_RB),
    ER2_WHL_LB_CLR = ~(0b11 << ER2_WHL_LB),
};

enum er2_wheel_mask {
    ER2_WHL_RF_MASK = 0b11 << ER2_WHL_RF,
    ER2_WHL_LF_MASK = 0b11 << ER2_WHL_LF,
    ER2_WHL_RB_MASK = 0b11 << ER2_WHL_RB,
    ER2_WHL_LB_MASK = 0b11 << ER2_WHL_LB
};

struct er2_wheel {
    enum er2_wheel_off offset;
    enum er2_wheel_st  state;

    rpp_pwm_t pwm;
};

struct er2_wheels {
    rpp_595_t sr;
    struct er2_wheel array[ER2_WHL_NCOUNT];
};

static enum er2_wheel_off off[ER2_WHL_NCOUNT] = {
    ER2_WHL_RF,
    ER2_WHL_LF,
    ER2_WHL_RB,
    ER2_WHL_LB
};

static enum rpp_pin pins[ER2_WHL_NCOUNT] = {
    RPP_PIN_PWM_RF,
    RPP_PIN_PWM_LF,
    RPP_PIN_PWM_RB,
    RPP_PIN_PWM_LB
};

u8 er2_wheel_mask[ER2_WHL_NCOUNT] = {
    (0b11 << ER2_WHL_RF) & 0xFF,
    (0b11 << ER2_WHL_LF) & 0xFF,
    (0b11 << ER2_WHL_RB) & 0xFF,
    (0b11 << ER2_WHL_LB) & 0xFF
};

u8 er2_wheel_clr[ER2_WHL_NCOUNT] = {
    ~(0b11 << ER2_WHL_RF) & 0xFF,
    ~(0b11 << ER2_WHL_LF) & 0xFF,
    ~(0b11 << ER2_WHL_RB) & 0xFF,
    ~(0b11 << ER2_WHL_LB) & 0xFF
};

static bool _wheel_init(struct er2_wheel * array, enum er2_wheel_idx idx) {
    array[idx].offset = off[idx];
    array[idx].state  = ER2_WHL_ST_S;

    array[idx].pwm = rpp_pwm_create(pins[idx], WHEELS_FREQ_HZ);
    if (!array[idx].pwm || !rpp_pwm_start(array[idx].pwm))
        return false;

    return true;
}

er2_wheels_t er2_whl_create(void) {
    struct er2_wheels * wheels = bxi_malloc(sizeof(struct er2_wheels));

    wheels->sr = rpp_595_create(RPP_PIN_SER, RPP_PIN_SRCLK, RPP_PIN_CLK);

    for(u8 i = 0; i < ER2_WHL_NCOUNT; ++i)
        if (!_wheel_init(wheels->array, i))
            er2_whl_destroy(wheels);

    return wheels;
}

inline static void _wheel_stop(struct er2_wheel * array,
                               enum er2_wheel_idx idx) {
    rpp_pwm_destroy(array[idx].pwm);
}

void er2_whl_destroy(er2_wheels_t wheels) {
    for(u8 i = 0; i < ER2_WHL_NCOUNT; ++i)
        _wheel_stop(wheels->array, i);

    rpp_595_destroy(wheels->sr);
    bxi_free(wheels);
}

void er2_whl_set(er2_wheels_t wheels,
                 enum er2_wheel_idx idx, enum er2_wheel_st state) {
    wheels->array[idx].state = state;

    rpp_595_set(wheels->sr, er2_wheel_clr[idx],
                (state << off[idx]) & er2_wheel_mask[idx]);
}

void er2_whl_update(er2_wheels_t wheels) {
    rpp_595_update(wheels->sr);
}

void er2_whl_setup(er2_wheels_t wheels,
                   enum er2_wheel_idx idx, enum er2_wheel_st state) {
    er2_whl_set(wheels, idx, state);
    er2_whl_update(wheels);
}

void er2_whl_state_set(er2_wheels_t wheels, struct er2_wheels_st * state) {
    er2_whl_set(wheels, ER2_WHL_RFI, state->rf);
    er2_whl_set(wheels, ER2_WHL_LFI, state->lf);
    er2_whl_set(wheels, ER2_WHL_RBI, state->rb);
    er2_whl_set(wheels, ER2_WHL_LBI, state->lb);
}

void er2_whl_set_speed(er2_wheels_t wheels, u8 value) {
    value = 40 + 3 * value / 5;

    for(u8 i = 0; i < ER2_WHL_NCOUNT; ++i)
        rpp_pwm_update(wheels->array[i].pwm, value);
}
