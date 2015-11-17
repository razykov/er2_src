#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "hmc5883l.h"
#include <machine_control.h>
#include <wheels.h>

static int flag = 1;

static void 
sig_handler() {
  flag = 0;
  destroy_hmc5883l();
}

int 
main() {
  
  signal(SIGINT, sig_handler);
  
  init_wheels();
  init_hmc5883l();  
  daemon_on(ENABLE);
  
  nstep_yaw(2, RGH);
  while(0.0 > get_azimuth_grad() || get_azimuth_grad() > 2.0) {
    printf("azimuth1: %f\n", get_azimuth_grad());
    usleep(30000);
  }  
  stop();
  usleep(1000000);
  
  nstep_yaw(2, RGH);
  while(88.0 > get_azimuth_grad() || get_azimuth_grad() > 92.0) {
    printf("azimuth1: %f\n", get_azimuth_grad());
    usleep(30000);
  }  
  stop();
  usleep(1000000);
  
  nstep_yaw(2, RGH);
  while(178.0 > get_azimuth_grad() || get_azimuth_grad() > 182.0) {
    printf("azimuth1: %f\n", get_azimuth_grad());
    usleep(30000);
  }  
  stop();
  usleep(1000000);
  
  nstep_yaw(2, RGH);
  while(268.0 > get_azimuth_grad() || get_azimuth_grad() > 272.0) {
    printf("azimuth1: %f\n", get_azimuth_grad());
    usleep(30000);
  }  
  stop();
  usleep(1000000);
  
  return 0;  
}