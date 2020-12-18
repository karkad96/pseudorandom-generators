#!/bin/bash

if [ ! -e /dev/rnd_device ]; then
	sudo mknod /dev/rnd_device c 123 0
	sudo chmod 777 /dev/rnd_device
fi
