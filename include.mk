# Common includes in Makefile
#
# Copyright(C) 2015 Beihang University
# Written By Wang Zhengdong ( jueastwang@outlook.com )


CROSS_COMPILE := arm-none-eabi-
CC	      := $(CROSS_COMPILE)gcc
CFLAGS	      := -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -O -fno-builtin  -Wall -DRPI2=1
LD	      := $(CROSS_COMPILE)ld
OBJCOPY	      := $(CROSS_COMPILE)objcopy
# FOR RPI2 ONLY!
