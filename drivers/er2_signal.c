/*
 * Kernel module for er2_controller LED signals generation.
 *
 * Author:
 * 	Razykov Vyacheslav <v.razykov@gmail.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */


// #########################################
// #                                       #
// #   shutdown             â              #
// #    ________            â              #
// #   |        |           â              #
// #   |        |         | â Repeat x10   #
// #  _|        |_________| â              #
// #   <-250ms->.<-200ms->. â              #
// #                                       #
// #                                       #
// #########################################
// #
   #define SHTDWN_HI_TIME	250
   #define SHTDWN_LOW_TIME	200
   #define SHTDWN_COUNT		10
// #
// #########################################

//#define NDEBUG

#define DEVICE_NAME	"er2_signals"
#define DYNAMIC_MAJOR	0
#define BUF_LEN		1024

#define HIGHT	1
#define LOW	0


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <asm/uaccess.h>
#include "er2.h"


extern struct class* er2_class;

static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user * buffer,
                           size_t length, loff_t * offset);
static ssize_t device_write(struct file *file, const char __user * buffer, 
                            size_t length, loff_t * offset);

static void repeat_sequence(unsigned int hi_time, unsigned int low_time,
                            unsigned int count);
static void er2_ledsig_shutdown(void);

static struct cdev dev_sled;
static dev_t dev_no;
static int device_is_open = FALSE; 
static char message[BUF_LEN];
static char* message_ptr;

struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release,
};


/*
 * Module init function
 */
static int __init
er2_signal_init(void)
{
	int ret = 0;

	ret = alloc_chrdev_region( &dev_no , 0, 1, DEVICE_NAME);
	if (ret < 0) {
		printk(KERN_ERR "er2_signal: error of allocate a range of char device");
		
		return ret;
	}

	cdev_init(&dev_sled, &fops);
	ret = cdev_add(&dev_sled, dev_no, 1);
	if (ret < 0) {
		class_destroy(er2_class);
        	unregister_chrdev_region(dev_no, 1);
		
		printk(KERN_ERR "er2_signal: error while added device to the system");
		
		return ret;
	}

	if ( device_create(er2_class, NULL, dev_no, NULL, DEVICE_NAME) == NULL ) {
		cdev_del(&dev_sled);
        	class_destroy(er2_class);
        	unregister_chrdev_region(dev_no, 1);		

		printk(KERN_ERR "er2_signal: error while device creating");
		
		return FAILURE;
	}

#ifndef NDEBUG
	printk(KERN_DEBUG "er2_signal: module started");
#endif

	return SUCCESS;
}

/*
 * Module exit function
 */
static void __exit
er2_signal_exit(void)
{
	// device unregister
	device_destroy(er2_class, dev_no);
	cdev_del(&dev_sled);
	unregister_chrdev_region(dev_no, 1);

#ifndef NDEBUG
        printk(KERN_DEBUG "er2_signal: module exit");
#endif
}

static int
device_open(struct inode *inode, struct file *file)
{
	if (device_is_open) {
		printk(KERN_WARNING "er2_signal: device is busy");
		return -EBUSY;
	}
	
	device_is_open = TRUE;
	
	message_ptr = message;
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

static int
device_release(struct inode *inode, struct file *file)
{
	device_is_open = FALSE;
	
	module_put(THIS_MODULE);
	return SUCCESS;
}


static ssize_t
device_read(struct file *file, char __user * buffer,
            size_t length, loff_t * offset)
{
	return -EPERM;
}


static ssize_t
device_write(struct file *file, const char __user * buffer, 
             size_t length, loff_t * offset)
{
	int i;
	
	for (i = 0; i < length && i < BUF_LEN; ++i)
	{
		get_user(message[i], buffer + i);
		message[i] = toupper(message[i]);
		if(message[i] == '\n')
			message[i] = '\0';
	}
	
	message_ptr = message;

	if( strcmp( message, "SHUTDOWN") == 0)
		er2_ledsig_shutdown();

	return i;
}

static void 
repeat_sequence(unsigned int hi_time, unsigned int low_time, unsigned int count)
{
	int i = 0;
        for(i = 0; i < count; ++i) {
                gpio_set_value(SIGNAL_LED, HIGHT);
                msleep(hi_time);
                gpio_set_value(SIGNAL_LED, LOW);
                msleep(low_time); 
        }
}

static void
er2_ledsig_shutdown(void)
{
	repeat_sequence(SHTDWN_HI_TIME, SHTDWN_LOW_TIME, SHTDWN_COUNT);
}


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Razykov Vyacheslav");
MODULE_DESCRIPTION("Kernel module for er2_controller LED signals generation.");

module_init(er2_signal_init);
module_exit(er2_signal_exit);
