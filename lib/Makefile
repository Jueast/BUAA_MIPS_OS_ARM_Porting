INCLUDES := -I./ -I../ -I../include/
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: print.o printf.o rpi-armtimer.o rpi-aux.o rpi-gpio.o rpi-interrupts.o\
      rpi-systimer.o
clean:
	rm -rf *.o *~


include ../include.mk
