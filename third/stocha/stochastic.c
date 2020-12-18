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
int fd, rnd_fd, count_fd;

void printError(char* str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void open_files()
{
	if ((fd = open("/dev/rnd_device", O_RDWR)) < 0)
		printError("open char device");

	if ((rnd_fd = open("/dev/urandom", O_RDONLY)) == 0)
		printError("open dev random");
	
	if ((count_fd = open("./count_times", O_RDWR | O_CREAT, 0777)) == 0)
		printError("open dev random");
}

void get_random_number(const char* str)
{
	if(write(fd, str, strlen(str)) < 0)
		printError("write to char device");
	
	if(read(fd, buff, 10) < 0)
		printError("read from char device");
}

int main (int argc, char* argv[]) 
{
    	int random_number;
	int times[3] = {0};	
	char counts[SIZE] = {0};
	
	open_files();

	for (int j = 0; j < 450000; j++)
	{
		if (read(rnd_fd, &random_number, sizeof(random_number) - 1) < 0)
			printError("read from dev random");

		switch (random_number % 3)
		{
			case 0:
				get_random_number("SOBOL");
				times[0]++;
				break;
			case 1:
				get_random_number("HALTON");
				times[1]++;
				break;
			case 2:
				get_random_number("LCG");
				times[2]++;
				break;
			default:
				break;
		}
	}	

	sprintf(counts, "%d,%d,%d", times[0], times[1], times[2]);
		
	if(write(count_fd, counts, strlen(counts)) < 0)
		printError("write");
	
	if (close(fd) < 0)
		printError("close char device");

	return 0;
}
