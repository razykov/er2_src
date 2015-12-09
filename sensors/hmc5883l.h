#ifndef _HMC5883L_H
#define _HMC5883L_H

#include <math.h>

enum state_e {DISABLE, ENABLE};

extern int init_hmc5883l();
extern void destroy_hmc5883l();
extern void filter_on(enum state_e st);
extern void daemon_on(enum state_e st);
extern double get_azimuth_rad();

extern double 
inline get_azimuth_grad()
{
  return get_azimuth_rad() * 180.0 / M_PI + 180.0;  
}

#endif /* _HMC5883L_H */