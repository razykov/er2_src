/**
 @file
 @brief
 @en Methods for wheels control.
 @ru Методы для управления шасси.
 */

#ifndef WHEELS_H
#define WHEELS_H

/**
 @brief
 @en Initialization wheel module. It need be called before using other functions from this module.
 @ru Инициализация модуля wheels. Необходимо вызвать перед использованием других функций из этого модуля.
 @return
 @en Code of error. 0 if no erros.
 @ru Код ошибки. 0 если ошибок нет.
 */
extern int init_wheels(void);

/**
 @brief 
 @en Set the speed of rotation foreach wheels to zero and set stop state for it.
 @ru Снижает скорость вращения всех колес до нуля и переводит их в stop состояние.
 @return
 @en Code of error. 0 if no erros.
 @ru Код ошибки. 0 если ошибок нет.
 */
extern int set_stop_state(void);

/**
 @brief 
 @en Set the speed of rotation foreach wheels to zero and set forth moving state for it.
 @ru Снижает скорость вращения всех колес до нуля и переводит их в состояние движения вперед.
 @return
 @en Code of error. 0 if no erros.
 @ru Код ошибки. 0 если ошибок нет.
 */
extern int set_forth_state(void);

/**
 @brief 
 @en Set the speed of rotation foreach wheels to zero and set back moving state for it.
 @ru Снижает скорость вращения всех колес до нуля и переводит их в состояние движения назад.
 @return
 @en Code of error. 0 if no erros.
 @ru Код ошибки. 0 если ошибок нет.
 */
extern int set_back_state(void);

/**
 @brief 
 @en Set the speed of rotation foreach wheels to zero and set state clockwise rotation for robot platform.
 @ru Снижает скорость вращения всех колес до нуля и переводит их в состояние поворота корпуса робота по часовой стрелке.
 @return
 @en Code of error. 0 if no erros.
 @ru Код ошибки. 0 если ошибок нет.
 */
extern int set_rrot_state(void);

/**
 @brief 
 @en Set the speed of rotation foreach wheels to zero and set state counterclockwise rotation for robot platform.
 @ru Снижает скорость вращения всех колес до нуля и переводит их в состояние поворота корпуса робота против часовой стрелки.
 @return
 @en Code of error. 0 if no erros.
 @ru Код ошибки. 0 если ошибок нет.
 */
extern int set_lrot_state(void);

#endif /* WHEELS_H */
