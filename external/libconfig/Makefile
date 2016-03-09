PREFIX	= /home/asiris/workbench/pi/wifi/rtl8192eu_rpi2/workbench/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-
CC	= $(PREFIX)gcc

CFLAGS	= -Wall -I. -O2
LIB	= libconfig.a

SRC	= \
	libconfig.c \
	scanctx.c \
	scanner.c \
	strbuf.c \
	grammar.c

OBJ	= $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJ)
	$(PREFIX)ar rcs $@ $^
	$(PREFIX)ranlib $@

clean:
	rm -f *.o $(LIB)

