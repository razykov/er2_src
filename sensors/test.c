#include <stdio.h>
#include "hmc5883l.h"

int main() {
  
  init_hmc5883l();
  
  daemon_on(ENABLE);
  
  destroy_hmc5883l();
  
  return 0;  
}