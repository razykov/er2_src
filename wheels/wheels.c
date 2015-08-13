#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
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

#define SET_LOW( pin )         (*(gpio + GPIOTOGPCLR) = 1 << (pin & 0x1F))
#define SET_HIG( pin )         (*(gpio + GPIOTOGPSET) = 1 << (pin & 0x1F))

// Switching to complementary state
#define COMPL_SWITCH( CMPL )   (*(gpio + ( CMPL == LW ? GPIOTOGPCLR : GPIOTOGPSET ) ) = 1 << (SIG_GPIO_PIN  & 0x1F))
#define COMPL_SWITCH_LW        (*(gpio + GPIOTOGPCLR) = 1 << (SIG_GPIO_PIN  & 0x1F))
#define COMPL_SWITCH_UP        (*(gpio + GPIOTOGPSET) = 1 << (SIG_GPIO_PIN  & 0x1F))

/* TODO: export to header with global macros */
#define SIG_GPIO_PIN        17
#define CLK_GPIO_PIN        27

enum states {FORTH, BACK, RROT, LROT} state;
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



int set_move_state(void);
int set_velocity_zero(void);


static void inline reg_clk(void) {
    SET_HIG(CLK_GPIO_PIN);
    SET_LOW(CLK_GPIO_PIN);
}

static int inline change_state(enum states s, enum states cs, enum compls t) {
    
    if(state != s) {
        
        if(set_velocity_zero())
            return ERROR_STOPED_ALL_WHEELS;
        
        if(state != cs)
            set_move_state();
        else
            COMPL_SWITCH(t);
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
    
    return 0;
}

int set_rotate_state(void) {
    
    int rotate_state[NUMBER_MOTOR_PINS] = {ROTATE_STATE_ARRAY};
    int gpio_shift[] = {GPIOTOGPCLR, GPIOTOGPSET};    
    
    int i;
    for(i = 0; i < NUMBER_MOTOR_PINS; ++i) {
        *(gpio + gpio_shift[rotate_state[i]]) = 1 << (SIG_GPIO_PIN  & 0x1F);
        reg_clk();
    }
    
    return 0;
}

/* TODO: finish after pwm control realisation */
int set_velocity_zero(void) {
    
    return 0;
}




int set_stop_state(void) {
    
    if(set_velocity_zero())
        return ERROR_STOPED_ALL_WHEELS;
    
    int i;
    SET_LOW(SIG_GPIO_PIN);
    for(i = 0; i < NUMBER_MOTOR_PINS; ++i)
        reg_clk();
    
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
