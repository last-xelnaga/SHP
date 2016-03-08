CFLAGS=-Wall -I. -O2
LIB=libconfig.a

SRC=libconfig.c \
    scanctx.c \
    scanner.c \
    strbuf.c \
    grammar.c

OBJ=$(SRC:.c=.o)

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $^
	ranlib $@

clean:
	rm -f *.o $(LIB)

