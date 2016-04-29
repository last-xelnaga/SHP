PREFIX	=

CC	= $(PREFIX)g++
AR	= $(PREFIX)ar
RANLIB	= $(PREFIX)ranlib
INC	= -I. -Iinclude
CFLAGS	= -Wall -O2 -Wno-missing-field-initializers

SRC	= \
	src/gtest_main.cc \
	src/gtest-all.cc

OBJ=$(SRC:.cc=.o)

%.o: %.cc
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: all clean

all: libgtest

libgtest: $(OBJ)
	$(AR) rcs $@.a $(OBJ)
	$(RANLIB) $@.a

clean:
	rm -f src/*.o libgtest.a
