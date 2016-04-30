PREFIX	=

CC	= $(PREFIX)g++
INC	= -I.. -I../external/gtest/include
CFLAGS	= -Wall -O2 -fno-omit-frame-pointer -DDEBUG -DDEBUG_TAG="---TEST---"
#CFLAGS	+= -DTRACE -DDATA
LIB	= -L../external/gtest -lgtest -lpthread

SRC	= \
	socket_test.cpp \
	../socket.cpp \
	../debug.cpp

OBJ=$(SRC:.cpp=.o)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: all clean

all: test_shared

test_shared: $(OBJ)
	$(CC) -o $@ $^ $(LIB)

clean:
	rm -f *.o ../*.o test_shared
