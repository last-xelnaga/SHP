
PREFIX	=
-include ../../makefile.prefix
CC	= $(PREFIX)gcc

CFLAGS	= -Wall -Ilib -O2
LIB	= libconfig.a

SRC	= \
	lib/libconfig.c \
	lib/scanctx.c \
	lib/scanner.c \
	lib/strbuf.c \
	lib/grammar.c

OBJ	= $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJ)
	$(PREFIX)ar rcs $@ $^
	$(PREFIX)ranlib $@

clean:
	rm -f lib/*.o $(LIB)
