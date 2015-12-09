/*
################################################################################
#                                                                              #
#   shutdown             ╔══╗╔══╗ ╔═════╗                                      #
#    ________            ╚═╗║║╔═╝ ║ ╔═══╝                                      #
#   |        |           ──║╚╝║── ║ ╚═══╗                                      #
#   |        |           ──║╔╗║── ╚═══╗ ║                                      #
#  _|        |_________  ╔═╝║║╚═╗ ╔═══╝ ║                                      #
#   <-250ms->.<-200ms->. ╚══╝╚══╝ ╚═════╝                                      #
#                                                                              #
################################################################################
*/


#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>
#include "er_signal.h"
#include "../general/gpio.h"
#include "../general/er_errors.h"


#define GPIO_BASE	(BCM2708_PERI_BASE + 0x00200000)

#define BLOCK_SIZE	(4*1024)

#define GPIOTOGPSET	7
#define GPIOTOGPCLR	10

#define SET_OUT( pin )          ( *(gpio + gpioToGPFSEL[pin]) = (*(gpio + gpioToGPFSEL[pin]) & ~(7 << gpioToShift  [pin])) | (1 << gpioToShift  [pin]) )

#define SET_LOW( pin )	(*(gpio + GPIOTOGPCLR) = 1 << (pin & 0x1F))
#define SET_HIG( pin )	(*(gpio + GPIOTOGPSET) = 1 << (pin & 0x1F))

#define LED_PIN	4


// gpioToGPFSEL:
//	Map a BCM_GPIO pin to it's Function Selection
//	control port. (GPFSEL 0-5)
//	Groups of 10 - 3 bits per Function - 30 bits per port

static uint8_t gpioToGPFSEL [] = {
    0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3,3,3,
    4,4,4,4,4,4,4,4,4,4,
    5,5,5,5,5,5,5,5,5,5,
} ;

// gpioToShift
//	Define the shift up for the 3 bits per pin in each GPFSEL port

static uint8_t gpioToShift [] = {
    0,3,6,9,12,15,18,21,24,27,
    0,3,6,9,12,15,18,21,24,27,
    0,3,6,9,12,15,18,21,24,27,
    0,3,6,9,12,15,18,21,24,27,
    0,3,6,9,12,15,18,21,24,27,
} ;

extern volatile uint32_t *gpio;


int 
init_signal()
{
  init_gpio();
  
  SET_OUT(LED_PIN);
  SET_LOW(LED_PIN);
  
  return 0;
}

void lsig_shutdown()
{
  const int light_ittr = 5;
  
  int i;
  for(i = 0; i < light_ittr; ++i) {
    SET_HIG(LED_PIN);
    usleep(250000);
    SET_LOW(LED_PIN);
    usleep(200000);
  }
}