#ifndef _OPERATIONS_H_ 
#define _OPERATIONS_H_  

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>

extern void sobol(char* output_str);
extern void halton(char* output_str, int base);
extern void LCG(char* output_str);

#define SIZE 2048

static char random_number[SIZE];
static char input[SIZE];

int my_open(struct inode *inode, struct file *filep);
int my_release(struct inode *inode, struct file *filep);
ssize_t my_read(struct file *filep, char *buff,size_t count, loff_t *offp );
ssize_t my_write(struct file *filep, const char *buff,size_t count, loff_t *offp );

struct file_operations my_fops={
    	open:    my_open,
    	read:    my_read,
	write:   my_write,
	release: my_release,
};

int my_open(struct inode *inode, struct file *filep)
{
	return 0;
}

int my_release(struct inode *inode, struct file *filep)
{
	return 0;
}

ssize_t my_read(struct file *filep, char *buff, size_t count, loff_t *offp)
{
	ssize_t output_str_size = 0;
	
	if(!strcmp(input, "SOBOL"))
		sobol(random_number);
	else if(!strcmp(input, "HALTON"))
		halton(random_number, 2);
	else if(!strcmp(input, "LCG"))
		LCG(random_number);
	else
		random_number[0] = '\0';

	output_str_size = strlen(random_number);

	if (copy_to_user(buff, random_number, output_str_size) < 0)
    		printk(KERN_INFO "Kernel -> userspace copy failed!\n");
	
	return output_str_size;

}

ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp)
{
	if (copy_from_user(input, buff, count) != 0)
    		printk(KERN_INFO "Userspace -> kernel copy failed!\n");

	input[count] = '\0';

	return 0;
}

#endif
