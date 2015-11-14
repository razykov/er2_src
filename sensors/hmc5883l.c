#define _GNU_SOURCE
#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <assert.h>
#include "hmc5883l.h"

#define TRUE	(1 == 1)
#define FALSE	(!TRUE)

#define I2C_ADDR_MIN	0x03
#define I2C_ADDR_MAX	0x77

#define DO_0_75Hz	(0b00000000)
#define DO_1_5Hz	(0b00000100)
#define DO_3Hz		(0b00001000)
#define DO_7_5Hz	(0b00001100)
#define DO_15Hz		(0b00010000)
#define DO_30Hz		(0b00010100)
#define DO_75Hz		(0b00011000)

#define MA_1		(0b00000000)
#define MA_2		(0b00100000)
#define MA_4		(0b01000000)
#define MA_8		(0b01100000)

#define MS_MODE_NORMAL	(0b00000000)
#define MS_MODE_POS	(0b00000001)
#define MS_MODE_NEG	(0b00000010)

#define GAIN_0_88	(0b00000000)
#define GAIN_1_30	(0b00100000)
#define GAIN_1_90	(0b01000000)
#define GAIN_2_50	(0b01100000)
#define GAIN_4_00	(0b10000000)
#define GAIN_4_70	(0b10100000)
#define GAIN_5_60	(0b11000000)
#define GAIN_8_10	(0b11100000)

#define GAIN_VALID	(0b11100000)

#define HS_MODE		(0b10000000)
#define LS_MODE		(0b00000000)
#define CM_MODE		(0b00000000)
#define SM_MODE		(0b00000001)
#define IDLE_MODE	(0b00000011)

#define HMC5883L_I2C_ADDR 	(0x1E)

#define I2C_FILE	"/dev/i2c-1"
#define DEVICE_NAME	"HMC5883L"
#define WR_BUF_SIZE	(2)
#define CONFIG_A 	0x00
#define CONFIG_B 	0x01
#define MODE 0x02
// #define DATA 0x03		//read 6 bytes: x msb, x lsb, z msb, z lsb, y msb, y lsb
// #define STATUS 0x09
#define ID_A 0x0A
// #define ID_B 0x0B
// #define ID_C 0x0C
#define ID_STRING 	"H43"
#define ID_STRING_LEN	(3)

static int sensor_detect (int fd);
static int select_device (int fd, int addr, char *name);
static int write_to_device (int fd, int reg, int val);
static int i2c_addr_valid (int addr);


static enum state_e filter;

int
init_hmc5883l()
{
  int fd;
  
  if ((fd = open (I2C_FILE, O_RDWR)) < 0) {
    // Open port for reading and writing
    fprintf (stderr, "Failed to open i2c bus\n");
    return EXIT_FAILURE;
  }
  
  select_device (fd, HMC5883L_I2C_ADDR, DEVICE_NAME);
  
  if(sensor_detect(fd) == EXIT_FAILURE) {
    fprintf (stderr, "Unknown sensor. Exiting.\n");
    return EXIT_FAILURE;
  }
  
  //Configuration
  int res = TRUE;
  //writeToDevice(fd, 0x01, 0);
  res &= write_to_device (fd, CONFIG_A, MA_8 | DO_3Hz | MS_MODE_NORMAL);
  res &= write_to_device (fd, CONFIG_B,  GAIN_0_88 & GAIN_VALID);
  res &= write_to_device (fd, MODE, LS_MODE | IDLE_MODE);
  
  return (res == TRUE) ? EXIT_SUCCESS : EXIT_FAILURE;    
}

void
filter_on(enum state_e st)
{
  filter = st;
}

void
daemon_on(enum state_e st)
{
  
}

double
get_azimuth()
{
  
}

static int
sensor_detect(int fd)
{
  char buf[16];
  
  //first read the 3 ID bytes
  buf[0] = ID_A;
  if ((write (fd, buf, 1)) != 1) {
    // Send the register to read from
    fprintf (stderr, "ERROR: Writing to i2c slave\n");
    return EXIT_FAILURE;
  }
  
  if (read (fd, buf, ID_STRING_LEN) != ID_STRING_LEN) {
    fprintf (stderr, "ERROR: Unable to read from HMC5883L\n");
    return EXIT_FAILURE;
  }
  
  buf[3] = '\0';
  printf ("Identification: '%s'\n", buf);
  if (strncmp (buf, ID_STRING, 3) == 0)
    return EXIT_SUCCESS;
    //printf ("HMC5883L sensor detected\n");
  else    
    return EXIT_FAILURE;
}

static int
select_device (int fd, int addr, char *name)
{
  if(!i2c_addr_valid(addr)){
    printf("ERROR: Incorrect address - %d\n", addr);
    return EXIT_FAILURE;
  }
  
  if (ioctl (fd, I2C_SLAVE, addr) < 0) {
    fprintf (stderr, "ERROR: %s not present\n", name);
    return EXIT_FAILURE;
  }
  else
    return EXIT_SUCCESS;
}

static int
write_to_device (int fd, int reg, int val)
{
  char wr_buf[WR_BUF_SIZE] = {reg, val};
  assert(wr_buf[0] == reg && wr_buf[1] == val);
  
  if (write (fd, wr_buf, WR_BUF_SIZE) != WR_BUF_SIZE) {
    fprintf (stderr, "ERROR: Can't write to HMC5883L\n");
    return EXIT_FAILURE;
  } 
  else
    return EXIT_SUCCESS;
}

static int
i2c_addr_valid(int addr)
{
  const int inv = addr & ~0x03;  
  
  return (addr < 0x03) || ((addr & ~0x7F) == 0x00) || 
	 (inv == 0x04) || (inv == 0x7C) || (inv == 0x79);
}
