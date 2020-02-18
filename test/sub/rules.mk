
CC=arm-linux-gcc
TR=arm-linux-strip
CCC=arm-linux-g++
AR=arm-linux-ar
LD=arm-linux-ld
NM=arm-linux-nm

MAKE = make

INCLUDE_PATH= ../include
MBUS_PATH= ../mbus

CFLAGS = -Wall -Werror -O0 -g -s -rdynamic -funwind-tables -ffunction-sections -I$(WORK_DIR) -I$(INCLUDE_PATH) -I$(MBUS_PATH)
DFLAGS = -D DEVICE_TYPE_JZQ1

DEPS := $(patsubst %.o, %.c.dep, $(OBJS))
TEMPDEPS := $(addsuffix ., $(dir $(DEPS)))
DEPS := $(join $(TEMPDEPS), $(notdir $(DEPS)))

IGNORE=$(wildcard .*.c.dep)
-include $(IGNORE)


	
%.o: %.c
	@echo "  CC  " $< "-o" $@; \
	$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@ -Wp,-MMD,$(dir $<).$(notdir $<).dep

