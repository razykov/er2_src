/**
 @file
 @brief
 @en Methods for controlling the four-wheel chassis with a parallel arrangement of two fixed wheelsets
 @ru Методы для управления четырехколесным шасси с параллельным расположением двух фиксированных колесных пар
 */

#ifndef _MACHINE_CONTROL_H
#define _MACHINE_CONTROL_H

enum yaw_direct {RGH, LFT};
enum speed_direct {UP, DW};

/**
 @brief
 @en Stop motion
 @ru Остановка движения
 @return
 @en Code of error. 0 if no erros.
 @ru Код ошибки. 0 если ошибок нет.
 */
extern int stop ( void );

/**
 @brief
 @en Step of change yaw
 @ru Шаг изменения рыскания шасси
 @param drct
 @en The direction of change. Use value LFT for turn to the left and use value RGH for turn to the right.
 @ru Направление изменения. Значение LFT для отклонения в левую сторону и RGH в правую.
 @return
 @en Code of error. 0 if no erros.
 @ru Код ошибки. 0 если ошибок нет.
 */
extern int step_yaw ( enum yaw_direct drct );

/**
 @brief
 @en Step of change velocity
 @ru Шаг изменения скорости поступательного движения
 @param drct
 @en The direction of change. Use value UP for velocity increase and use value DW for deceleration.
 @ru Направление изменения. Значение UP для увеличения скорости, DW для ее уменьшения.
 @return
 @en Code of error. 0 if no erros.
 @ru Код ошибки. 0 если ошибок нет.
 */
extern int step_speed ( enum speed_direct drct );

/**
 @brief
 @en Few steps of change yaw
 @ru Несколько шагов изменения рыскания шасси
 @param n
 @en Number of steps. Max value - 5.
 @ru Количество шагов. Максимально возможное значение - 5.
 @param drct
 @en The direction of change. Use value LFT for turn to the left and use value RGH for turn to the right.
 @ru Направление изменения. Значение LFT для отклонения в левую сторону и RGH в правую.
 @return
 @en Code of error. 0 if no erros.
 @ru Код ошибки. 0 если ошибок нет.
 */
extern int nstep_yaw (unsigned int n, enum yaw_direct drct);

/**
 @brief
 @en Few steps of change velocity
 @ru Несколько шагов изменения скорости поступательного движения
 @param n
 @en Number of steps. Max value - 10.
 @ru Количество шагов. Максимально возможное значение - 10.
 @param drct
 @en The direction of change. Use value UP for velocity increase and use value DW for deceleration.
 @ru Направление изменения. Значение UP для увеличения скорости, DW для 
 @return 
 @en Code of error. 0 if no erros.
 @ru Код ошибки. 0 если ошибок нет.
 */
extern int nstep_speed (unsigned int n, enum speed_direct drct);

#endif /* _MACHINE_CONTROL_H */
