/*
 * er2 core kernel module.
 *
 * Author:
 *      Razykov Vyacheslav <v.razykov@gmail.com>
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


#include <linux/module.h>
#include <linux/kernel.h>
#include "er2.h"


struct class* er2_class;
EXPORT_SYMBOL_GPL(er2_class);


static int __init
er2_core_init(void)
{
	int ret;

	er2_class = class_create(THIS_MODULE, "er2");
	if (er2_class == NULL) {
		printk(KERN_ERR "er2_signal: error  while device class creating");
		return FAILURE;
	}
	
	// register LED GPIOs
	ret = gpio_request_one(SIGNAL_LED, GPIOF_OUT_INIT_LOW, SIGNAL_LED_LABEL);
	if (ret) {
		printk(KERN_ERR "er2_signal: unable to request GPIOs: %d\n", ret);
		return FAILURE;
        }

	return SUCCESS;
}


static void __exit
er2_core_exit(void)
{
	class_destroy(er2_class);
	
	// turn LED off
	// unregister all GPIOs
	gpio_set_value(SIGNAL_LED, LOW);
	gpio_free(SIGNAL_LED);	
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Razykov Vyacheslav");
MODULE_DESCRIPTION("er2 core kernel module.");

module_init(er2_core_init);
module_exit(er2_core_exit);
