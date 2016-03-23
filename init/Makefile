INCLUDES := -I../include

%.o: %.c
	$(CC) $(DEFS) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: cstartup.o main.o

clean:
	rm -rf *.o *~


include ../include.mk

