#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "wheels.h"
#include "machine_control.h"

int main() {
    
    init_wheels();
    
    //nstep_speed(2, UP);    
    nstep_yaw(2, RGH);
    usleep(20.0 * 1000000);
    
    //nstep_speed(4, DW);
    nstep_yaw(2, LFT);
    //usleep(5.0 * 1000000);
    
    stop();
    
    return 0;
}
 
