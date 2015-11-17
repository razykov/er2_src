#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <linux/i2c-dev.h>

const int HMC5883L_I2C_ADDR = 0x1E;

#define CONFIG_A 0x00
#define CONFIG_B 0x01
#define MODE 0x02
#define DATA 0x03		//read 6 bytes: x msb, x lsb, z msb, z lsb, y msb, y lsb
#define STATUS 0x09
#define ID_A 0x0A
#define ID_B 0x0B
#define ID_C 0x0C
#define ID_STRING "H43"

#define GAIN 1370		//000 setting

#define HMC_ADDR	"calibration"

FILE *fd_hmc;

void
signalHandler() 
{
  fclose(fd_hmc);
  
  exit(0);
}

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
main (int argc, char **argv)
{
  int fd;
  unsigned char buf[16];
  struct timeval tv;
  struct timezone tz;		//ignored
  struct timeval data_timestamp;	//
  int resolution = 10000;	//microseconds
  long next_timestamp;

  
  signal( SIGINT, signalHandler );
  
  if ((fd = open ("/dev/i2c-1", O_RDWR)) < 0)
    {
      // Open port for reading and writing
      fprintf (stderr, "Failed to open i2c bus\n");

      return 1;
    }

  /* initialise HMC5883L */

  selectDevice (fd, HMC5883L_I2C_ADDR, "HMC5883L");

  //first read the 3 ID bytes
  buf[0] = ID_A;
  if ((write (fd, buf, 1)) != 1)
    {
      // Send the register to read from
      fprintf (stderr, "Error writing to i2c slave\n");
    }

  if (read (fd, buf, 3) != 3)
    {
      fprintf (stderr, "Unable to read from HMC5883L\n");
    }
  buf[3] = 0;
  printf ("Identification: '%s' ", buf);
  if (strncmp (buf, ID_STRING, 3) == 0)
    printf ("HMC5883L sensor detected\n");
  else
    {
      printf ("unknown sensor. Exiting.\n");
      exit (1);
    }

  //Configuration

  //writeToDevice(fd, 0x01, 0);
  writeToDevice (fd, CONFIG_A, 0b01101000);	//8 sample averaging
  writeToDevice (fd, CONFIG_B, 0b00000000);	//max gain
  writeToDevice (fd, MODE, 0b00000011);	//idle mode

  //find current time
  gettimeofday (&data_timestamp, &tz);
  data_timestamp.tv_sec += 1;	//start loggin at start of next second
  data_timestamp.tv_usec = 0;

  short max_x = 0, min_x = 0;
  short max_y = 0, min_y = 0;
  short max_z = 0, min_z = 0;
  int count = 0;
  
  fd_hmc = fopen( HMC_ADDR, "w");
  
  while (1)
    {				//record forever   

      //read time & wait until next reading required

      while (count < 1000)
	{
	  //get time
	  gettimeofday (&tv, &tz);

	  //if seconds >= next_secs && usecs >= next_usecs
	  if (tv.tv_sec >= data_timestamp.tv_sec
	      && tv.tv_usec >= data_timestamp.tv_usec)
	    break;

	  usleep (1000);
	}

      //initiate single conversion
      writeToDevice (fd, MODE, 0b00000001);	//single measurement

      //wait 7 milliseconds
      usleep (7000);

      buf[0] = DATA;

      if ((write (fd, buf, 1)) != 1)
	{
	  // Send the register to read from
	  fprintf (stderr, "Error writing to i2c slave\n");
	}

      if (read (fd, buf, 6) != 6)
	{
	  fprintf (stderr, "Unable to read from HMC5883L\n");
	}
      else
	{
          short x = (buf[0] << 8) | buf[1];
	  short y = (buf[4] << 8) | buf[5];
	  short z = (buf[2] << 8) | buf[3];
          
	  max_x = (x > max_x) ? count = 0, x : ++count, max_x;
	  min_x = (x < min_x) ? count = 0, x : ++count, min_x;
	  max_y = (y > max_y) ? count = 0, y : ++count, max_y;
	  min_y = (y < min_y) ? count = 0, y : ++count, min_y;
	  max_z = (z > max_z) ? count = 0, z : ++count, max_z;
	  min_z = (z < min_z) ? count = 0, z : ++count, min_z;
	}
	
      printf( "%d %d\n%d %d\n%d %d\n", max_x, min_x, max_y, min_y, max_z, min_z);
      fprintf( fd_hmc, "%d %d\n%d %d\n%d %d\n", max_x, min_x, max_y, min_y, max_z, min_z);
      
      //advance data timestamp to next required time
      data_timestamp.tv_usec += resolution;
      if (data_timestamp.tv_usec >= 1e6)
	{
	  data_timestamp.tv_sec += 1;
	  data_timestamp.tv_usec -= 1e6;
	}

    }

  return 0;
}
