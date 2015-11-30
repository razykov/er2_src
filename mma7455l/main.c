#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#define TRUE	(1 == 1)

// The 'raw' 7 bit address shifted up 
#define MMA7455L_I2C_ADDR	(0x1D)

// The registers to read!
#define MMA7455_XOUT8 0x06
#define MMA7455_YOUT8 0x07
#define MMA7455_ZOUT8 0x08

#define G_CONST	9.80665
#define SMOOTH	0.1

#define _2G_POINTS_PER_G	64
#define _4G_POINTS_PER_G	32
#define _8G_POINTS_PER_G	16

#define I2C_FILE_NAME "/dev/i2c-1"

void
selectDevice (int fd, int addr, char *name)
{
  if (ioctl (fd, I2C_SLAVE, addr) < 0)
    {
      fprintf (stderr, "%s not present\n", name);
      //exit(1);
    }
}

void
writeToDevice (int fd, int reg, int val)
{
  char buf[2];
  buf[0] = reg;
  buf[1] = val;

  if (write (fd, buf, 2) != 2)
    {
      fprintf (stderr, "Can't write to ADXL345\n");
      //exit(1);
    }
}

int
main(int argc, char **argv) {
  
  int fd, i;
  signed char buf[6];
    
  if ((fd = open ("/dev/i2c-1", O_RDWR)) < 0){
    // Open port for reading and writing
    fprintf (stderr, "Failed to open i2c bus\n");
    
    return 1;
  }
  
  selectDevice (fd, MMA7455L_I2C_ADDR, "MMA7455L");
  
  writeToDevice (fd, 0x16, 0x45);
  
  for(i = 0x10; i < 0x16; ++i)
    writeToDevice(fd, i, 0);
  //writeToDevice(fd, 0x10, 12);
  //writeToDevice(fd, 0x12, 29);
  
  double ema_x = 0.0, ema_y = 0.0, ema_z = 0.0; 
  double dma_x = 0.0, dma_y = 0.0, dma_z = 0.0;
  double tma_x = 0.0, tma_y = 0.0, tma_z = 0.0;
  
  while(TRUE) {
      buf[0] = 0x06;
      
      if ((write (fd, buf, 1)) != 1) {
          // Send the register to read from
          printf ("Error writing to i2c slave\n");
      }
      
      if (read (fd, buf, 3) != 3) {
          printf ("Unable to read from MMA7455L\n");
      }
      else
      {
// 	double x = (((short int)buf[1] << 8) | buf[0]);
// 	double y = (short int)(buf[3] << 8) | buf[2];
// 	double z = (short int)(buf[5] << 8) | buf[4];
	signed char ix = buf[0];
	signed char iy = buf[1];
	signed char iz = buf[2];
	
	double x = G_CONST * buf[0] / _2G_POINTS_PER_G;
	double y = G_CONST * buf[1] / _2G_POINTS_PER_G;
	double z = G_CONST * buf[2] / _2G_POINTS_PER_G;
	
	ema_x = SMOOTH * x + (1 - SMOOTH) * ema_x;
	ema_y = SMOOTH * y + (1 - SMOOTH) * ema_y;
	ema_z = SMOOTH * z + (1 - SMOOTH) * ema_z;
	
	dma_x = SMOOTH * ema_x + (1 - SMOOTH) * dma_x;
	dma_y = SMOOTH * ema_y + (1 - SMOOTH) * dma_y;
	dma_z = SMOOTH * ema_z + (1 - SMOOTH) * dma_z;
	
	tma_x = SMOOTH * dma_x + (1 - SMOOTH) * tma_x;
	tma_y = SMOOTH * dma_y + (1 - SMOOTH) * tma_y;
	tma_z = SMOOTH * dma_z + (1 - SMOOTH) * tma_z;
	
	printf("%f %f %f\n", tma_x, tma_y, tma_z);
      }
      usleep(10000);
  }
  
}