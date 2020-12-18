#include "operations.h"

MODULE_AUTHOR("KarolK");
MODULE_DESCRIPTION("random number generator char device");
MODULE_LICENSE("GPL v2");

static int my_init(void)
{
	printk(KERN_INFO "Hi\n");

	if(register_chrdev(123, "rnd_device", &my_fops))
    		printk(KERN_INFO "failed to register\n");

	return 0;
}

static void my_cleanup(void)
{
	printk(KERN_INFO "Bye\n");

	unregister_chrdev(123, "rnd_device");

	return;
}

module_init(my_init);
module_exit(my_cleanup);
