# Main makefile
#
# Copyright (C) 2016 Beihang University
# Written by Wang Zhengdong ( jueastwang@outlook.com )
#
# inspired by Zhu Like's work
# ========= Configuration =========

INCLUDES          := -I../include/

# ======== End of configuration ===

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c $< 

.PHONY: clean

all: start.o

clean:
	rm -rf *.o *~

include ../include.mk

