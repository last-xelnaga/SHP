PREFIX	=

OBJDIR = ../obj/obj_proxy
TARGET = ../shp_proxy
CC	= $(PREFIX)g++

CFLAGS	= -Wall -Wextra -DDEBUG_TAG="proxy"
CFLAGS	+= -O2 -fno-omit-frame-pointer
CFLAGS	+= -O0 -ggdb
CFLAGS	+= -DDEBUG
#CFLAGS	+= -DTRACE -DDATA

INC = -I.
LIB = -lpthread

# main
SRC	= \
    proxy_config.cpp \
    proxy_event.cpp \
    proxy_main.cpp \
    proxy_version.cpp \
    debug.cpp \
    message.cpp \
    queue.cpp \
    socket_server.cpp \
    socket.cpp

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
$(OBJDIR)/proxy_config.o: proxy_main.hpp message.hpp socket_server.hpp debug.hpp error_codes.hpp proxy.makefile
$(OBJDIR)/proxy_event.o: proxy_main.hpp message.hpp socket_server.hpp debug.hpp error_codes.hpp proxy.makefile
$(OBJDIR)/proxy_main.o: proxy_main.hpp message.hpp queue.hpp socket_server.hpp debug.hpp error_codes.hpp proxy.makefile
$(OBJDIR)/proxy_version.o: proxy_main.hpp message.hpp socket_server.hpp debug.hpp error_codes.hpp proxy.makefile
$(OBJDIR)/debug.o: debug.hpp error_codes.hpp proxy.makefile
$(OBJDIR)/message.o: message.hpp debug.hpp error_codes.hpp proxy.makefile
$(OBJDIR)/queue.o: queue.hpp message.hpp debug.hpp error_codes.hpp proxy.makefile
$(OBJDIR)/socket_server.o: socket_server.hpp socket.hpp message.hpp debug.hpp error_codes.hpp proxy.makefile
$(OBJDIR)/socket.o: socket.hpp debug.hpp error_codes.hpp proxy.makefile
