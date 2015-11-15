#ifndef _HMC5883L_H
#define _HMC5883L_H

enum state_e {DISABLE, ENABLE};

extern int init_hmc5883l();
extern void filter_on(enum state_e st);
extern void daemon_on(enum state_e st);
extern double get_azimuth_rad();

#endif /* _HMC5883L_H */