# Main makefile
#
# Copyright (C) 2016 Beihang University
# Written by Wang Zhengdong ( jueastwang@outlook.com )
#
# inspired by Zhu Like's work
#

drivers_dir  	:= drivers
boot_dir 	:= boot
init_dir	:= init
lib_dir		:= lib
drivers_dir     := drivers
tools_dir	:= tools
output_tool     := rpi-aux
kernel_elf	:= RPI2/kernel.elf
kernel_img 	:= RPI2/kernel.img

link_script 	:= $(tools_dir)/rpi.x

modules		:= boot init lib drivers
objects 	:= $(boot_dir)/start.o			\
		   $(init_dir)/*.o			\
		   $(lib_dir)/*.o			\
		   $(drivers_dir)/raspberrypi2/*.o 			

.PHONY: all $(modules) clean

all: $(modules) RPI2


RPI2: $(modules)
	 $(LD) -o  $(kernel_elf) -N -T $(link_script) $(objects) /usr/lib/gcc/arm-none-eabi/4.8/armv7-ar/thumb/libgcc.a
	$(OBJCOPY) $(kernel_elf) -O binary $(kernel_img)
$(modules):
	$(MAKE) --directory=$@

clean:
	for d in $(modules);	\
		do		\
			$(MAKE) --directory=$$d  clean; \
		done; \

include include.mk

