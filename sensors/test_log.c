#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "hmc5883l.h"


static int flag = 1;

static void 
sig_handler() {
  flag = 0;
  destroy_hmc5883l();
}

int
main() {
  
  signal(SIGINT, sig_handler);
  
  init_hmc5883l();  
  daemon_on(ENABLE);
  
  while(flag) {
    //printf("azimuth1: %f\n", get_azimuth_grad());
    usleep(100000);
  }
  
  return 0;  
}