PREFIX	=

-include makefile.prefix

OBJDIR  = ../obj/obj_head
TARGET  = ../shp_head

CC	= $(PREFIX)g++

CFLAGS	= -Wall -Wextra -DDEBUG_TAG="head"
#CFLAGS	+= -O2 -fno-omit-frame-pointer
CFLAGS	+= -O0 -ggdb
CFLAGS	+= -DDEBUG
#CFLAGS	+= -DTRACE -DDATA


INC = -I.
LIB = -lpthread

# wiringPi
INC += -Iexternal/wiringPi/wiringPi
LIB += -Lexternal/wiringPi/wiringPi -lwiringPi

# main
SRC	+= \
    head_config.cpp \
    head_event.cpp \
    head_main.cpp \
    debug.cpp \
    message.cpp \
    socket_client.cpp \
    socket_server.cpp \
    socket.cpp \
    queue.cpp

OBJ = $(SRC:%.cpp=$(OBJDIR)/%.o)

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: all clean

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJDIR) $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIB)

clean:
	rm -rf $(OBJDIR) $(TARGET)

# main
$(OBJDIR)/head_config.o: debug.hpp message.hpp client_sensor_manager.hpp queue.hpp socket_client.hpp error_codes.hpp head.makefile
$(OBJDIR)/head_event.o: debug.hpp message.hpp client_sensor_manager.hpp queue.hpp socket_client.hpp error_codes.hpp head.makefile
$(OBJDIR)/head_main.o: debug.hpp message.hpp client_sensor_manager.hpp queue.hpp socket_client.hpp error_codes.hpp head.makefile
$(OBJDIR)/debug.o: debug.hpp error_codes.hpp head.makefile
$(OBJDIR)/message.o: message.hpp debug.hpp error_codes.hpp head.makefile
$(OBJDIR)/queue.o: queue.hpp message.hpp debug.hpp error_codes.hpp head.makefile
$(OBJDIR)/socket_client.o: socket_client.hpp socket.hpp message.hpp debug.hpp error_codes.hpp head.makefile
$(OBJDIR)/socket.o: socket.hpp debug.hpp error_codes.hpp head.makefile
