
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "wheels.h"
#include "diagnostics.h"

int main() {
    
    //init_diagnostics();;
    init_wheels();
    
    //forth_back
    /*
    set_forth_state();
    set_wheels_velocity(80);
    usleep(1 * 1000000);
    set_wheels_velocity(0);
    
    set_back_state();
    set_wheels_velocity(80);
    usleep(1 * 1000000);
    set_wheels_velocity(0);
    
    set_stop_state();
    */
    
    
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
    
    return 0;
}
 
