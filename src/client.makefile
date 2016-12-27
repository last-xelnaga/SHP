PREFIX	=

-include makefile.prefix

REVFILE = REVISION
OBJDIR  = ../obj/obj_client
TARGET  = ../shp_client

CC	= $(PREFIX)g++

CFLAGS	= -Wall -Wextra -DDEBUG_TAG="client"
#CFLAGS	+= -O2 -fno-omit-frame-pointer
CFLAGS	+= -O0 -ggdb
CFLAGS	+= -DDEBUG
#CFLAGS	+= -DTRACE -DDATA


INC = -I.
LIB = -lpthread

# libconfig
INC	+= -Iexternal/libconfig/lib
LIB	+= -Lexternal/libconfig -lconfig

# wiringPi
INC += -Iexternal/wiringPi/wiringPi
LIB += -Lexternal/wiringPi/wiringPi -lwiringPi

# zlib
INC += -Iexternal/zlib
LIB += -Lexternal/zlib -lz


# sensors
SRC	= \
    sensors/sensor_event.cpp \
    sensors/sensor_led.cpp \
    sensors/sensor_dht.cpp \
    sensors/sensor_relay.cpp \
    sensors/sensor_servo.cpp

# main
SRC	+= \
    client_config.cpp \
    client_main.cpp \
    client_process_config.cpp \
    client_process_version.cpp \
    client_sensor_manager.cpp \
    debug.cpp \
    message.cpp \
    queue.cpp \
    socket_client.cpp \
    socket.cpp \
    version.cpp

OBJ = $(SRC:%.cpp=$(OBJDIR)/%.o)

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: all clean

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)/sensors

$(REVFILE): $(SRC)
	@if ! test -f $(REVFILE); then echo 0 > $(REVFILE); fi
	@echo $$(($$(cat $(REVFILE)) + 1)) > $(REVFILE)
	@touch version.cpp
	@make simple=y -f client.makefile

$(TARGET): $(REVFILE) $(OBJDIR) $(OBJ)
ifndef simple
	$(CC) -o $@ $(OBJ) $(LIB)
	@./../zpipe  < $(TARGET) > ../$$(echo $$(($$(cat $(REVFILE))))).gz
endif

clean:
	rm -rf $(OBJDIR) $(TARGET)

# sensors
$(OBJDIR)/sensors/sensor_led.o: sensors/sensor_led.hpp sensors/sensor.hpp client.makefile
$(OBJDIR)/sensors/sensor_dht.o: sensors/sensor_dht.hpp sensors/sensor_event.hpp sensors/sensor.hpp client.makefile
$(OBJDIR)/sensors/sensor_event.o: sensors/sensor_event.hpp sensors/sensor.hpp client.makefile
$(OBJDIR)/sensors/sensor_relay.o: sensors/sensor_relay.hpp sensors/sensor.hpp client.makefile
$(OBJDIR)/sensors/sensor_servo.o: sensors/sensor_servo.hpp sensors/sensor.hpp client.makefile

# main
$(OBJDIR)/client_config.o: client_config.hpp debug.hpp message.hpp error_codes.hpp client.makefile
$(OBJDIR)/client_main.o: client_config.hpp debug.hpp message.hpp client_sensor_manager.hpp queue.hpp socket_client.hpp error_codes.hpp client.makefile
$(OBJDIR)/client_process_config.o: debug.hpp message.hpp socket_client.hpp error_codes.hpp client.makefile
$(OBJDIR)/client_process_version.o: debug.hpp message.hpp socket_client.hpp error_codes.hpp client.makefile
$(OBJDIR)/debug.o: debug.hpp error_codes.hpp client.makefile
$(OBJDIR)/message.o: message.hpp debug.hpp error_codes.hpp client.makefile
$(OBJDIR)/queue.o: queue.hpp message.hpp debug.hpp error_codes.hpp client.makefile
$(OBJDIR)/client_sensor_manager.o: client_sensor_manager.hpp sensors/sensor_event.hpp sensors/sensor_led.hpp sensors/sensor_dht.hpp sensors/sensor_relay.hpp sensors/sensor.hpp queue.hpp client_config.hpp debug.hpp error_codes.hpp client.makefile
$(OBJDIR)/socket_client.o: socket_client.hpp socket.hpp message.hpp debug.hpp error_codes.hpp client.makefile
$(OBJDIR)/socket.o: socket.hpp debug.hpp error_codes.hpp client.makefile
$(OBJDIR)/version.o: version.hpp VERSION
