
drivers		:= raspberrypi2

.PHONY: all $(drivers)

all: $(drivers)

$(drivers):
	$(MAKE) --directory=$@

clean:
	for d in $(drivers); \
                do                              \
                        $(MAKE) --directory=$$d clean; \
                done

