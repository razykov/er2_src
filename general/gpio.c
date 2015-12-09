#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdio.h>
#include "gpio.h"
#include "../general/er_errors.h"

#define BCM2708_PERI_BASE	0x20000000
#define GPIO_BASE		(BCM2708_PERI_BASE + 0x00200000)

#define BLOCK_SIZE		(4*1024)


volatile uint32_t *gpio = 0;

int
init_gpio()
{
  if(gpio == 0 || gpio == MAP_FAILED) {
    
    int fd;
    
    // Open the master /dev/memory device
    if ( ( fd = open ( "/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC ) ) < 0 )
      return ERROR_UNABLE_TO_OPEN_MEM;
    
    gpio = ( uint32_t* ) mmap ( 0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE );
    if ( gpio == MAP_FAILED )
      return ERROR_MMAP_GPIO_FAILED;
  }
  
  return 0;    
}