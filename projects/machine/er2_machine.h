#ifndef ER2_MACHINE_H
#define ER2_MACHINE_H

#include <libbixi.h>
#include "er2_wheels.h"

EXPORT_FROM

extern er2_wheels_st_t whls_stop;
extern er2_wheels_st_t whls_forth;
extern er2_wheels_st_t whls_back;
extern er2_wheels_st_t whls_lrot;
extern er2_wheels_st_t whls_rrot;
extern er2_wheels_st_t whls_right;
extern er2_wheels_st_t whls_left;

typedef struct er2_machine * er2_machine_t;

er2_machine_t er2_machine_create(void);

void er2_machine_destroy(er2_machine_t machine);

void er2_mch_wheels_state(er2_machine_t machine, er2_wheels_st_t * state);

void er2_mch_set_speed(er2_machine_t machine, u8 speed);

EXPORT_TO

#endif /* ER2_MACHINE_H */
