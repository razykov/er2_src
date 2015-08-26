#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h> //
#include <unistd.h> //
#include "wheels.h"
#include "er_errors.h"


#define BCM2708_PERI_BASE       0x20000000
#define GPIO_BASE               (BCM2708_PERI_BASE + 0x00200000)

#define BLOCK_SIZE              (4*1024)

#define NUMBER_WHEELS           4
#define NUMBER_WHEELS_PAIRS     2
#define NUMBER_MOTOR_PINS       8

/* WARNING: only for Raspberry PI B rev2.
 *           For other versions use arrays gpioToGPSET and gpioToGPCLR 
 */
#define GPIOTOGPSET             7
#define GPIOTOGPCLR             10

#define ROTATE_STATE_ARRAY      0,1,1,0,0,1,1,0

#define SET_OUT( pin )          ( *(gpio + gpioToGPFSEL[pin]) = (*(gpio + gpioToGPFSEL[pin]) & ~(7 << gpioToShift  [pin])) | (1 << gpioToShift  [pin]) )

#define SET_LOW( pin )          (*(gpio + GPIOTOGPCLR) = 1 << (pin & 0x1F))
#define SET_HIG( pin )          (*(gpio + GPIOTOGPSET) = 1 << (pin & 0x1F))

// Switching to complementary state
#define COMPL_SWITCH_MOVE( CMPL )   (*(gpio + ( (CMPL == LW) ? GPIOTOGPCLR : GPIOTOGPSET ) ) = 1 << (SIG_GPIO_PIN  & 0x1F))
#define COMPL_SWITCH_ROTATE(CMPL)   {\
                                        *(gpio + ( (CMPL == LW) ? GPIOTOGPSET : GPIOTOGPCLR ) ) = 1 << (SIG_GPIO_PIN  & 0x1F);\
                                        reg_clk();\
                                        *(gpio + ( (CMPL == LW) ? GPIOTOGPCLR : GPIOTOGPSET ) ) = 1 << (SIG_GPIO_PIN  & 0x1F);\
                                    }
#define COMPL_SWITCH_LW        (*(gpio + GPIOTOGPCLR) = 1 << (SIG_GPIO_PIN  & 0x1F))
#define COMPL_SWITCH_UP        (*(gpio + GPIOTOGPSET) = 1 << (SIG_GPIO_PIN  & 0x1F))

/* TODO: export to header with global macros */
#define SIG_GPIO_PIN        17
#define CLK_GPIO_PIN        27
#define RCLK_GPIO_PIN       22

#define PWM_LB_GPIO_PIN     1
#define PWM_RB_GPIO_PIN     4
#define PWM_LF_GPIO_PIN     5
#define PWM_RF_GPIO_PIN     6

enum states {FORTH, BACK, RROT, LROT, STOP} state;
enum compls {LW = 0, HI = 1};

static volatile uint32_t *gpio;

//(Word) offset to the GPIO Set registers for each GPIO pin
// static uint8_t gpioToGPSET [] =
// {
//    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
//    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
// };

//(Word) offset to the GPIO Clear registers for each GPIO pin
// static uint8_t gpioToGPCLR [] =
// {
//   10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
//   11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
// };

// gpioToGPFSEL:
//	Map a BCM_GPIO pin to it's Function Selection
//	control port. (GPFSEL 0-5)
//	Groups of 10 - 3 bits per Function - 30 bits per port

static uint8_t gpioToGPFSEL [] =
{
  0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,
  4,4,4,4,4,4,4,4,4,4,
  5,5,5,5,5,5,5,5,5,5,
} ;

// gpioToShift
//	Define the shift up for the 3 bits per pin in each GPFSEL port

static uint8_t gpioToShift [] =
{
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
} ;



int set_move_state(void);
int set_rotate_state(void);


static void inline reg_clk(void) {
    SET_HIG(CLK_GPIO_PIN); usleep(1);
    SET_LOW(CLK_GPIO_PIN); usleep(1);
}

static void inline reg_rclk(void) {
    SET_HIG(RCLK_GPIO_PIN);  usleep(1);
    SET_LOW(RCLK_GPIO_PIN);  usleep(1);
}

static int inline change_state(enum states s, enum states cs, enum compls t) {

#ifndef NDEBUG    
    printf("change_state: state=%d, s=%d,cs=%d, t=%d\n", state, s, cs, t);
#endif
    
    if(set_wheels_velocity(0) != 0)
        return ERROR_STOPED_ALL_WHEELS;
    
    if(state != s) {
                
        if(set_velocity_zero() != 0)
            return ERROR_STOPED_ALL_WHEELS;
                      
        if(state == cs) {
            if(state == FORTH || state == BACK)
                COMPL_SWITCH_MOVE(t);
            else
                COMPL_SWITCH_ROTATE(t);
                          
            reg_clk();
        }
        else
            switch(s) {
                case FORTH:
                    set_move_state();
                    break;
                case BACK:
                    set_move_state();
                    COMPL_SWITCH_MOVE(t);
                    reg_clk();
                    break;
                case RROT:
                    set_rotate_state();
                    break;
                case LROT:
                    set_rotate_state();
                    COMPL_SWITCH_ROTATE(t);
                    reg_clk();
                    break;
                default:
                    break;
            }
                        
        reg_rclk();
        state = s;
    }
    
    return 0;
}



int init_wheels(void) {
    
    int fd;
    
    // Open the master /dev/memory device
    if ((fd = open ("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC) ) < 0)
        return ERROR_UNABLE_TO_OPEN_MEM;
    
    gpio = (uint32_t*)mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
    if ((int32_t)gpio == -1)
        return ERROR_MMAP_GPIO_FAILED;
        
    
    SET_OUT(SIG_GPIO_PIN);
    SET_OUT(CLK_GPIO_PIN);
    SET_OUT(RCLK_GPIO_PIN);
    
    
    wiringPiSetup ();
    
    softPwmCreate(PWM_LB_GPIO_PIN, 0, 100);
    softPwmCreate(PWM_RB_GPIO_PIN, 0, 100);
    softPwmCreate(PWM_LF_GPIO_PIN, 0, 100);
    softPwmCreate(PWM_RF_GPIO_PIN, 0, 100);
    
    state = STOP;
    
#ifndef NDEBUG
    printf("wheels initiazed\n");
#endif
        
    return 0;
}

int set_move_state(void) {
    
    int i;
    for(i = 0; i < NUMBER_WHEELS; ++i) {
        SET_HIG(SIG_GPIO_PIN);
        reg_clk();
        SET_LOW(SIG_GPIO_PIN);
        reg_clk();
    }
    
#ifndef NDEBUG
    printf("set_move_state\n");
#endif
        
    return 0;
}

int set_rotate_state(void) {
    
    int rotate_state[] = {ROTATE_STATE_ARRAY};
    int gpio_shift[] = {GPIOTOGPCLR, GPIOTOGPSET};    
    
    int i;
    for(i = 0; i < NUMBER_MOTOR_PINS; ++i) {
        *(gpio + gpio_shift[rotate_state[i]]) = 1 << (SIG_GPIO_PIN  & 0x1F);
        reg_clk();
    }
    
    reg_rclk();
    
#ifndef NDEBUG
    printf("set_rotate_state\n");
#endif
    
    return 0;
}

int set_wheels_velocity(int new_value) {
    
    if(new_value < 0 || new_value > 100) new_value = 0;
    
    static int pwm_value = 0;    
    int value_change = (new_value < pwm_value ? -1 : 1);
    
    while(pwm_value != new_value) {
        pwm_value += value_change;
        softPwmWrite(PWM_LB_GPIO_PIN, pwm_value);
        softPwmWrite(PWM_RB_GPIO_PIN, pwm_value);
        softPwmWrite(PWM_LF_GPIO_PIN, pwm_value);
        softPwmWrite(PWM_RF_GPIO_PIN, pwm_value);
        usleep(10000);
    }
    
    return 0;
}

int set_stop_state(void) {
    
    if(set_wheels_velocity(0) != 0)
        return ERROR_STOPED_ALL_WHEELS;
    
    int i;
    SET_LOW(SIG_GPIO_PIN);
    for(i = 0; i < NUMBER_MOTOR_PINS; ++i) 
        reg_clk();
    reg_rclk(); 
    
    state = STOP;
    
    return 0;
}

int set_forth_state(void) {    
    return change_state(FORTH, BACK, LW);
}

int set_back_state(void) {    
    return change_state(BACK, FORTH, HI);
}

int set_rrot_state(void) {
    return change_state(RROT, LROT, LW);
}

int set_lrot_state(void) {    
    return change_state(LROT, RROT, HI);
}
