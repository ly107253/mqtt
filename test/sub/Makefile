
WORK_DIR := $(shell pwd)
MQTT_OUTPATH=../lib

LINKFLAG := 
OBJS := main.o 


MAINOUT := lysub.out
XZIPOUT := lysub.xzip
SRIPOUT := lysubtmp
MAINCRC := lysub
XMAINCRC := lysub.xzip

SHARELIBS := -lpthread -L../lib -lmbus 

all: information  $(MAINOUT)

-include rules.mk

.PHONY: information
information:
	@$(CC) -v
	@echo "device = "$(DFLAGS)

$(MAINOUT): $(OBJS)
	@echo "  make  "$@"..."
	@./utils/mkdate ./mkdate.c
	@$(CC) $(CFLAGS) $(DFLAGS) -c mkdate.c -o mkdate.o
	@$(CC) $(OBJS)  mkdate.o $(LINKFLAG) $(SHARELIBS) -o $@
	@rm -f mkdate.c mkdate.o
	@echo "  make  "$@" ok"
	@cp -f $(MAINOUT) $(SRIPOUT)
	@$(NM) $(SRIPOUT) >> symbols.txt
	@$(TR) $(SRIPOUT)
	@./utils/xzip z 3699 $(SRIPOUT) $(XZIPOUT)
	@./utils/appendcrc $(SRIPOUT) $(MAINCRC)
	@./utils/xzip z 3699 $(MAINCRC) $(XMAINCRC)
	@cp $(MAINCRC) $(MQTT_OUTPATH)
clean:
	@rm -f $(MAINOUT) $(XZIPOUT) $(SRIPOUT) $(MAINCRC) $(XMAINCRC) $(OBJS) $(DEPS) *.o .*.c.dep
	@echo remove objs ok



