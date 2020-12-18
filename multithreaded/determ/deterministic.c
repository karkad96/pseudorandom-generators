#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <pthread.h>

#define SIZE 2048

const int N = 8;

char buff[SIZE];

int fd, fd_file;

//pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void printError(char* str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void open_files()
{
	if ((fd = open("/dev/rnd_device", O_RDWR)) < 0)
		printError("open char device");
	
	if((fd_file = open("./test", O_RDWR | O_CREAT, 0777)) < 0)
		printError("open test file");
}

void get_random_number(const char* str, int tmp)
{
//	pthread_mutex_lock( &mutex1 );

	if(write(fd, str, strlen(str)) < 0)
		printError("write to char device");
	
	if(read(fd, buff, 10) < 0)
		printError("read from char device");

//	buff[10] = '\n';
//	buff[11] = '\0';

	printf("%s %d\n", buff, tmp);

//	if(write(fd_file, buff, strlen(buff)) < 0)
//		printError("write to test file");
	
//	pthread_mutex_unlock( &mutex1 );
}

void* read_from_thread(void* arg)
{
	int* s = (int*)arg;

	for(int j = 0; j < 15000 / N; j++)
	{
		get_random_number("SOBOL", *s);
	}

	pthread_exit(s);
}

int main(int argc, char* argv[]) 
{
	open_files();

	if(argc != 2)
	{
		printf("Wrong number of arguments!\n");
		exit(EXIT_FAILURE);
	}

//	pthread_mutex_init(&mutex1, NULL);

	pthread_t read_from[N];
	
	int read_i[N];

	for(int i = 0; i < N; i++)
	{
		read_i[i] = i;

		int result = pthread_create(&read_from[i], NULL, read_from_thread, &read_i[i]);

    		if (result != 0)
			printError("cannot create thread!");
	}

	for(int i = 0; i < N; i++)
	{	
		int* read_from_result;

		pthread_join(read_from[i], (void **) &read_from_result);
//		printf("Thread returned, %d.\n", *read_from_result);
	}

	if(close(fd) < 0)
		printError("close char device");

	if(close(fd_file) < 0)
		printError("close test file");

	return 0;
}
