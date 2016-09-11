#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

// The 'raw' 7 bit address shifted up 
#define MMA7455_I2CADDR (0x1D)

// The registers to read!
#define MMA7455_XOUT8 0x06
#define MMA7455_YOUT8 0x07
#define MMA7455_ZOUT8 0x08


#define I2C_FILE_NAME "/dev/i2c-1"

static int get_i2c_register(int fd,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char *val) {
    unsigned char inbuf, outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    /*
     * In order to read a register, we first do a "dummy write" by writing
     * 0 bytes to the register we want to read from.  This is similar to
     * the packet in set_i2c_register, except it's 1 byte rather than 2.
     */
    outbuf = reg;
    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = &outbuf;

    /* The data will get returned in this structure */
    messages[1].addr  = addr;
    messages[1].flags = I2C_M_RD/* | I2C_M_NOSTART*/;
    messages[1].len   = sizeof(inbuf);
    messages[1].buf   = &inbuf;

    /* Send the request to the kernel and get the result back */
    packets.msgs      = messages;
    packets.nmsgs     = 2;
    if(ioctl(fd, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        return 1;
    }
    *val = inbuf;

    return 0;
}


int main(int argc, char **argv) {
    int i2c_file;
    int8_t x, y, z;  // the readings are 8 bits and signed!

    x = y = z = 0;
    
    // Open a connection to the I2C userspace control file.
    if ((i2c_file = open(I2C_FILE_NAME, O_RDWR)) < 0) {
        perror("Unable to open i2c control file");
        exit(1);
    }

    // ignore arguments!

    while (1) {
      
      // read X and Y and Z from the register
      if( get_i2c_register(i2c_file, MMA7455_I2CADDR, MMA7455_XOUT8, &x) ||
	  get_i2c_register(i2c_file, MMA7455_I2CADDR, MMA7455_YOUT8, &y) ||
	  get_i2c_register(i2c_file, MMA7455_I2CADDR, MMA7455_ZOUT8, &z) ) {

	printf("Unable to read register!\n");
	return -1;
      }
      
      if(x != 0 || y != 0 || z != 0)
	printf("X = %d\tY = %d\tZ = %d\n", x, y, z); 
      
      //usleep(10000);
    }

    close(i2c_file);
    return 0;
}