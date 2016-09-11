#include "er_signal.h"

int
main()
{
  init_signal();
  
  lsig_shutdown();
  
  return 0;  
}