#include <librpi_peri.h>
#include "er2_machine.h"

struct er2_machine {
    er2_wheels_t wheels;
};

struct er2_wheels_st whls_stop = {
    .rf = ER2_WHL_ST_S,
    .lf = ER2_WHL_ST_S,
    .rb = ER2_WHL_ST_S,
    .lb = ER2_WHL_ST_S
};

struct er2_wheels_st whls_forth = {
    .rf = ER2_WHL_ST_F,
    .lf = ER2_WHL_ST_F,
    .rb = ER2_WHL_ST_F,
    .lb = ER2_WHL_ST_F
};

struct er2_wheels_st whls_back = {
    .rf = ER2_WHL_ST_B,
    .lf = ER2_WHL_ST_B,
    .rb = ER2_WHL_ST_B,
    .lb = ER2_WHL_ST_B
};

struct er2_wheels_st whls_rrot = {
    .rf = ER2_WHL_ST_B,
    .lf = ER2_WHL_ST_F,
    .rb = ER2_WHL_ST_B,
    .lb = ER2_WHL_ST_F
};

struct er2_wheels_st whls_lrot = {
    .rf = ER2_WHL_ST_F,
    .lf = ER2_WHL_ST_B,
    .rb = ER2_WHL_ST_F,
    .lb = ER2_WHL_ST_B
};

struct er2_wheels_st whls_right = {
    .rf = ER2_WHL_ST_B,
    .lf = ER2_WHL_ST_F,
    .rb = ER2_WHL_ST_F,
    .lb = ER2_WHL_ST_B
};

struct er2_wheels_st whls_left = {
    .rf = ER2_WHL_ST_F,
    .lf = ER2_WHL_ST_B,
    .rb = ER2_WHL_ST_B,
    .lb = ER2_WHL_ST_F
};

static void _blink(void) {
    for(u32 i = 0; i < 3; ++i) {
        rpp_gpio_set(RPP_PIN_LED, RPP_PIN_ST_HI);
        bxi_msleep(100);
        rpp_gpio_set(RPP_PIN_LED, RPP_PIN_ST_LW);
        bxi_msleep(100);
    }
}

er2_machine_t er2_machine_create(void) {
    struct er2_machine * machine;

    if (!rpp_init()) return NULL;

    machine = bxi_malloc(sizeof(struct er2_machine));
    machine->wheels = er2_whl_create();

    rpp_gpio_out(RPP_PIN_LED);

    _blink();

    return machine;
}

void er2_machine_destroy(er2_machine_t machine) {
    if (!machine) return;

    er2_whl_destroy(machine->wheels);
    rpp_free();
}

void er2_mch_wheels_state(er2_machine_t machine, er2_wheels_st_t * state) {
    er2_whl_state_set(machine->wheels, state);
    er2_whl_update(machine->wheels);
}

void er2_mch_set_speed(er2_machine_t machine, u8 speed) {
    er2_whl_set_speed(machine->wheels, speed);
}
