
#include <stdio.h>
#include <math.h>
#include "wheels.h"
#include "machine_control.h"

static int change_wheels_speed ();
static double __step_yaw ( enum yaw_direct drct );
static double __step_speed ( enum speed_direct drct );


#define YAW_STEP	        0.1
#define MAX_YAW_STEPS		5
#define YAW_UP_BOUND	        MAX_YAW_STEPS * YAW_STEP
#define CORRECT_COEF            2 // (int) (1.0 / YAW_UP_BOUND)
#define SPEED_STEP              10.0
#define MAX_SPEED_STEPS		10
#define SPEED_UP_BOUND          MAX_SPEED_STEPS * SPEED_STEP
#define SPEED_BOTTOM_BOUND      3 * SPEED_STEP
#define ZERO_SPEED              0.0
#define ZERO_YAW                0.0
#define INCORRECT               SPEED_UP_BOUND + YAW_UP_BOUND


static double yaw = ZERO_YAW;
static double speed = ZERO_SPEED;


int stop( void ) {
    yaw = ZERO_YAW;
    speed = ZERO_SPEED;
    return change_wheels_speed ();
}

static double __step_yaw ( enum yaw_direct drct ) {

    //static double yaw = ZERO_YAW;

    switch ( drct ) {
    case RGH:
        yaw = yaw < YAW_UP_BOUND ? yaw + YAW_STEP : yaw;
        break;
    case LFT:
        yaw = yaw > - YAW_UP_BOUND ? yaw - YAW_STEP : yaw;
        break;
    }

    return yaw;
}

static double __step_speed ( enum speed_direct drct ) {

    //static double speed = ZERO_SPEED;

    switch ( drct ) {
    case UP:
            if ( speed == - SPEED_BOTTOM_BOUND ) {
                speed = ZERO_SPEED;
            } else if ( speed == ZERO_SPEED ) {
                speed = SPEED_BOTTOM_BOUND;
            } else if ( speed < SPEED_UP_BOUND ) {
                speed += SPEED_STEP;
            }
        break;
    case DW:
        if ( speed == SPEED_BOTTOM_BOUND ) {
            speed = ZERO_SPEED;
        } else if ( speed == ZERO_SPEED ) {
            speed = -SPEED_BOTTOM_BOUND;
        } else if ( speed > - SPEED_UP_BOUND ) {
            speed -= SPEED_STEP;
        }
        break;
    }

    return speed;
}


inline int step_yaw (enum yaw_direct drct) {

    __step_yaw(drct);
    change_wheels_speed();
    return 0;
}

inline int step_speed (enum speed_direct drct) {

    __step_speed(drct);
    change_wheels_speed();
    return 0;
}

int nstep_yaw ( unsigned int n, enum yaw_direct drct ) {

    n = n > MAX_YAW_STEPS ? MAX_YAW_STEPS : n;

    int i;
    for ( i = 0; i < n; ++i )
        __step_yaw ( drct );

    change_wheels_speed ();

    return 0;
}

int nstep_speed ( unsigned int n, enum speed_direct drct ) {

    n = n > MAX_SPEED_STEPS ? MAX_SPEED_STEPS : n;

    int i;
    for ( i = 0; i < n; ++i )
        __step_speed ( drct );

    change_wheels_speed ();

    return 0;
}


static int change_wheels_speed () {

    //static double yaw = ZERO_YAW, speed = ZERO_SPEED;

    //yaw = ( y == INCORRECT ) ? yaw : y;
    //speed = ( s == INCORRECT ) ? speed: s;

    double left_pair_speed, right_pair_speed;

    if ( speed == ZERO_SPEED ) {
        if ( yaw > ZERO_YAW ) {
            set_rrot_state();
        } else if ( yaw < ZERO_YAW ) {
            set_lrot_state();
        } else {
            set_stop_state();
        }

        //printf ( "y:%f\n", yaw );
        right_pair_speed = left_pair_speed = CORRECT_COEF * fabs ( yaw ) * SPEED_UP_BOUND;

    } else {
        if ( speed > ZERO_SPEED ) {
            set_forth_state();
        } else {
            set_back_state();
        }

        right_pair_speed = fabs ( yaw > ZERO_YAW  ? ( 1.0 - yaw ) * speed : speed );
        left_pair_speed = fabs ( yaw < ZERO_YAW  ? ( 1.0 + yaw ) * speed : speed );
    }

    struct wheels_speed new_speed = { left_pair_speed, right_pair_speed,
               left_pair_speed, right_pair_speed
    };

    set_wheels_speed ( &new_speed );

    return 0;

}
