
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "wheels.h"
#include "machine_control.h"
#include "diagnostics.h"

int main() {
    
    init_wheels();
    
    /* wheels test */ 
    /*
    struct wheels_speed new_val = {30, 30, 30, 30};
    
    set_forth_state();
    set_wheels_speed(&new_val);
    usleep(1.5 * 1000000);
    
    set_rrot_state();
    set_wheels_speed(&new_val);
    usleep(1.5 * 1000000);
    
    set_forth_state();
    set_wheels_speed(&new_val);
    usleep(1.5 * 1000000);
    
    set_lrot_state();
    set_wheels_speed(&new_val);
    usleep(1.5 * 1000000);
    
    set_stop_state();
    */
    
    /* test machine_control */
    
    nstep_speed(2, UP);
    
    nstep_yaw(2, RGH);
    usleep(1.0 * 1000000);
    nstep_yaw(2, LFT);
    
    usleep(4.0 * 1000000);
    
       
    nstep_speed(4, DW);
    usleep(4.0 * 1000000);
    
    stop();
    
    nstep_speed(2, UP);
    usleep(2.0 * 1000000);
    stop();
    
    return 0;
}
 
