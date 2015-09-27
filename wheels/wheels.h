/**
 @file
 @brief
 @en Methods for wheels control.
 @ru Методы для управления шасси.
 */

#ifndef WHEELS_H
#define WHEELS_H

/**
 @struct wheels_speed
 @brief 
 @en Container with speed value of wheels.
     WARNING!!! Variables must be in the range from 0.0 to 100.0
 @ru Контейнер для хранения процентного значения скорости вращения колес. 
     ВНИМАНИЕ!!! Переменные должны находиться в диапазоне от 0.0 до 100.0
 @var wheels_speed::lb
 @en The speed of the left rear wheel
 @ru Скорость левого заднего колеса
 @var wheels_speed::rb
 @en The speed of the right rear wheel
 @ru Скорость правого заднего колеса
 @var wheels_speed::lf
 @en The speed of the left front wheel
 @ru Скорость левого переднего колеса
 @var wheels_speed::rf
 @en The speed of the right front wheel
 @ru Скорость правого переднего колеса
 */
struct wheels_speed {
  float lb, rb, lf, rf;  
};


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

/**
 @brief
 @en Set the speed of rotation all wheel to value.
 @ru Устанавливает скорость вращения всех колес в указанное значение.
 @return
 @en Code of error. 0 if no erros.
 @ru Код ошибки. 0 если ошибок нет.
 @param new_value
 @en Set to speed. Accepts values from 0 to 100, in the case of incorrectly arguments to stop the movement.
 @ru Устанавливаемое значение скорости вращения. Принимает значения от 0 до 100, в случае некоректных аргументов останавливает движение.
 */
extern int set_all_wheels_speed (float new_value);

/**
 @brief
 @en Set the speed of rotation foreach wheel to new values.
 @ru Устанавливает скорость вращения каждого колеса в указанные значения.
 @return
 @en Code of error. 0 if no erros.
 @ru Код ошибки. 0 если ошибок нет.
 @param &new_value
 @en Pointer to a structure wheels_speed that including new values of speed.
 @ru Указатель на структуру wheels_speed содержащую значения устанавливаемых скоростей.
 */
extern int set_wheels_speed(const struct wheels_speed* const new_value);

#endif /* WHEELS_H */
