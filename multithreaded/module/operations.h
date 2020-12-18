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
#include <linux/mutex.h>

extern void sobol(char* output_str, int size);
extern void halton(char* output_str, int base, int size);
extern void LCG(char* output_str, int size);

#define SIZE 2048

static char random_number[SIZE];
static char input[SIZE];
struct mutex rnd_mutex;

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

	//mutex_lock(&rnd_mutex);

	if(!strcmp(input, "SOBOL"))
		sobol(random_number, count);
	else if(!strcmp(input, "HALTON"))
		halton(random_number, 2, count);
	else if(!strcmp(input, "LCG"))
		LCG(random_number, count);
	else
		random_number[0] = '\0';

	output_str_size = strlen(random_number);

	if (copy_to_user(buff, random_number, output_str_size) < 0)
    		printk(KERN_INFO "Kernel -> userspace copy failed!\n");

	memset(random_number, '\0', output_str_size);

	//mutex_unlock(&rnd_mutex);

	return output_str_size;

}

ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp)
{
	//mutex_lock(&rnd_mutex);

	if (copy_from_user(input, buff, count) != 0)
    		printk(KERN_INFO "Userspace -> kernel copy failed!\n");

	input[count] = '\0';

	//mutex_unlock(&rnd_mutex);

	return 0;
}

#endif
