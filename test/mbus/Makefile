
CC=arm-linux-gcc
TR=arm-linux-strip
NM=arm-linux-nm

MQTT_INPATH=../include
MQTT_OUTPATH=../lib

CFLAGS=-c  -fpic -O0 -g -s -lstdc++ -rdynamic -funwind-tables -ffunction-sections -Wall -Werror -DDEBUG

LDFLAGS= -fpic -shared -L../lib -lmosquitto 


INCLUDE= -I$(MQTT_INPATH)

SOURCES_C= ./mbus.c
			
OBJECTS=$(SOURCES_C:.c=.o) 
EXECUTABLE=./libmbustmp
	
all: $(SOURCES_C) libmbus.xzip

.c.o:
#	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@
	$(CC) $(CFLAGS) -I$(MQTT_INPATH) -I$(shell pwd) $< -o $@
	@echo .c.o OK	
	
	
libmbus.xzip: $(EXECUTABLE)
	@./utils/xzip z 3699 $< $@
	@echo Make xzip $@ OK
	@$(NM) libmbustmp >> symbols.txt
	@$(TR) libmbustmp
	@./utils/appendcrc libmbustmp libmbus.so
	@echo Make addcrc $@ OK
	@cp libmbus.so $(MQTT_OUTPATH)

$(EXECUTABLE): $(OBJECTS) 	
	$(CC) $(LDFLAGS)  $^ -o $@
	@echo Make $@ OK
	
install:
	cp -rf ./libmbus.so ../../../../interfaces/bin
	
clean:
	rm -rf ./*.o ./libmbus.so ./libmbustmp ./libmbus.xzip ./symbols.txt
		   
