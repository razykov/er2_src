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

#define HMC_ADDR	"/var/www/er_server/hmc"

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

  int cnt = 0;
  fd_hmc = fopen( HMC_ADDR, "w");
  
  fprintf(fd_hmc, "# X Y Z aZ pX pY\n");
  
  while (1)
    {				//record forever   

      //read time & wait until next reading required

      while (1)
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
          
	  double angle_rad = atan2(y, x);
	  double angle_grad = angle_rad * 180 / M_PI;
	  
	  double pX = cos(angle_rad);
	  double pY = sin(angle_rad);
	  //angle = (x < 0 && y < 0) ? - atan2(y, x) * 180 / M_PI : atan2(y, x) * 180 / M_PI;
	  
	  //float mag = sqrt (x * x + y * y + z * z);          
	  
 	  fprintf( fd_hmc, "%d %d %d %d %.2f %.2f %.2f\n", cnt++, x, y, z, angle_grad, pX, pY );
          
	  //printf("x=%d, y=%d, z=%d\n", x, y, z);
	  //printf("angle = %0.1f, mag = %0.1f\n", angle,mag);
	  //printf("time: %ld.%06ld\n",tv.tv_sec, tv.tv_usec);
// 	  printf ("%ld.%06ld, %d, %d, %d, %0.1f nT\n", data_timestamp.tv_sec,
// 		  data_timestamp.tv_usec, x, y, z, 1e5 * mag / GAIN);
	  //fflush (stdout);
	}

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
