PREFIX	=

CC	= $(PREFIX)g++
INC	= -I.. -I../external/gtest/include
CFLAGS	= -Wall -O2 -fno-omit-frame-pointer -DDEBUG -DDEBUG_TAG="---TEST---"
#CFLAGS	+= -DTRACE -DDATA
LIB	= -L../external/gtest -lgtest -lpthread

SRC	= \
	../debug.cpp \
	../message.cpp \
	message_test.cpp \
	#../socket.cpp \
	socket_test.cpp


OBJ=$(SRC:.cpp=.o)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: all clean

all: test_shared

test_shared: $(OBJ)
	$(CC) -o $@ $^ $(LIB)

clean:
	rm -f *.o ../*.o test_shared

socket_test.o: socket_test.cpp ../socket.cpp ../socket.hpp ../debug.cpp ../debug.hpp
message_test.o: message_test.cpp ../message.cpp ../message.hpp ../debug.cpp ../debug.hpp
