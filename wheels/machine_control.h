
#ifndef _MACHINE_CONTROL_H
#define _MACHINE_CONTROL_H

enum yaw_direct {RGH, LFT};
enum speed_direct {UP, DW};

/**
 @brief
 @param
 @return
 */
extern int stop ();

/**
 @brief
 @param
 @return
 */
extern int step_yaw ( enum yaw_direct drct );

/**
 @brief
 @param
 @return
 */
extern int step_speed ( enum speed_direct drct );

/**
 @brief
 @param
 @return
 */
extern int nstep_yaw (unsigned int n, enum yaw_direct drct);

/**
 @brief
 @param
 @return
 */
extern int nstep_speed (unsigned int n, enum speed_direct drct);

#endif /* _MACHINE_CONTROL_H */
