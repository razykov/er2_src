#ifndef WHEELS_H
#define WHEELS_H

#include <libbixi.h>

enum er2_wheel_idx {
    ER2_WHL_RFI = 0,
    ER2_WHL_LFI = 1,
    ER2_WHL_RBI = 2,
    ER2_WHL_LBI = 3,
    ER2_WHL_NCOUNT
};

enum er2_wheel_st {
    ER2_WHL_ST_S = 0b00,
    ER2_WHL_ST_F = 0b01,
    ER2_WHL_ST_B = 0b10
};

struct er2_wheels_st {
    enum er2_wheel_st rf;
    enum er2_wheel_st lf;
    enum er2_wheel_st rb;
    enum er2_wheel_st lb;
};

typedef struct er2_wheels * er2_wheels_t;

EXPORT typedef struct er2_wheels_st er2_wheels_st_t;

er2_wheels_t er2_whl_create(void);

void er2_whl_destroy(er2_wheels_t whl);

void er2_whl_set(er2_wheels_t wheels,
                 enum er2_wheel_idx idx, enum er2_wheel_st state);

void er2_whl_setup(er2_wheels_t wheels,
                   enum er2_wheel_idx idx, enum er2_wheel_st state);

void er2_whl_update(er2_wheels_t wheels);

void er2_whl_state_set(er2_wheels_t wheels, struct er2_wheels_st * state);

void er2_whl_set_speed(er2_wheels_t wheels, u8 value);

#endif /* WHEELS_H */
