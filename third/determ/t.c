#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

#define SIZE 2048

char buff[SIZE];

int fd, fd_file;

double avg;

void printError(char* str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void open_files()
{
	if ((fd = open("/dev/rnd_device", O_RDWR)) < 0)
		printError("open char device");
}

void get_random_number(const char* str)
{
	if(write(fd, str, strlen(str)) < 0)
		printError("write to char device");
	
	if(read(fd, buff, 10) < 0)
		printError("read from char device");

	avg += strtof(buff, NULL);
}

int main(int argc, char* argv[]) 
{
	open_files();

	if(argc != 2)
	{
		printf("Wrong number of arguments!\n");
		exit(EXIT_FAILURE);
	}

	for(int i=0;i<100;i++)
	{
		for(int j = 0; j < 100000; j++)
		{
			get_random_number(argv[1]);
		}

		printf("%lf\n", avg / 100000.0);
		avg = 0;
	}	
		
	if(close(fd) < 0)
		printError("close char device");

	return 0;
}
