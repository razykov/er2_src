#ifndef MACHINE_H
#define MACHINE_H

#include <libbixi.h>

typedef struct er2_wheels_st er2_wheels_st_t;

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

#endif /* MACHINE_H */
