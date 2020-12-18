obj-m += rnd.o rnd_seq.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo insmod rnd_seq.ko
	sudo insmod rnd.ko
	
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	sudo rmmod rnd
	sudo rmmod rnd_seq
