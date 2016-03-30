# Makefile for raspberrypi2 base, uart and other modules
#
# Written by Wang Zhengdong, jueastwang@buaa.edu.cn
CROSS_COMPILE := arm-none-eabi-
CC            := $(CROSS_COMPILE)gcc
CFLAGS        := -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -O -fno-builtin  -Wall -DRPI2=1


%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $*.o
.PHONY: clean
all: rpi-armtimer.o rpi-aux.o rpi-systimer.o rpi-interrupts.o rpi-aux.o rpi-gpio.o

clean:
	rm -rf *.o *~

inculde: ../../include.mk
