#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "hmc5883l.h"
#include <machine_control.h>
#include <wheels.h>

#define	TEST2
#define N_STEP_YAW	2

static int flag = 1;

static void 
sig_handler() {
  flag = 0;
  destroy_hmc5883l();
}

double
delta(double a, double b) {
  double c;
  c = (a > b) ? a - b : b - a;
  return (c < 180) ? c : 360 - c;
}

int 
main() {
  
  signal(SIGINT, sig_handler);
  
  init_wheels();
  init_hmc5883l();  
  daemon_on(ENABLE);
  
#ifdef TEST1
  nstep_yaw(N_STEP_YAW, RGH);
  while(0.0 > get_azimuth_grad() || get_azimuth_grad() > 2.0) {
    printf("azimuth1: %f\n", get_azimuth_grad());
    usleep(3000);
  }  
  stop();
  usleep(1000000);
  
  nstep_yaw(N_STEP_YAW, RGH);
  while(88.0 > get_azimuth_grad() || get_azimuth_grad() > 92.0) {
    printf("azimuth1: %f\n", get_azimuth_grad());
    usleep(3000);
  }  
  stop();
  usleep(1000000);
  
  nstep_yaw(N_STEP_YAW, RGH);
  while(178.0 > get_azimuth_grad() || get_azimuth_grad() > 182.0) {
    printf("azimuth1: %f\n", get_azimuth_grad());
    usleep(3000);
  }  
  stop();
  usleep(1000000);
  
  nstep_yaw(N_STEP_YAW, RGH);
  while(268.0 > get_azimuth_grad() || get_azimuth_grad() > 272.0) {
    printf("azimuth1: %f\n", get_azimuth_grad());
    usleep(3000);
  }  
  stop();
  usleep(1000000);
#endif /* TEST1 */
  
  
  
#ifdef TEST2
  double az_new, eps = 2;
  
  usleep(1000000);
  while(get_azimuth_grad() == 0.0);
  
  int i;
  az_new = get_azimuth_grad();
  for(i = 0; i < 4; ++i) {    
    az_new += 90;
    az_new = (az_new > 360) ? az_new - 360 : az_new;
    nstep_yaw(N_STEP_YAW, RGH); 
    
    while (delta(get_azimuth_grad(), az_new) > eps) {
      usleep(1000);
    }
    
    stop();
    usleep(1000000);
  }
#endif /* TEST2 */
  
  return 0;  
}