PREFIX	=

-include Make_client

CC	= $(PREFIX)g++
CFLAGS	= -Wall -O2 -fno-omit-frame-pointer
#CFLAGS+=-Wall -ggdb
INC = -I.
LIB = -lpthread

# libconfig
INC	+= -Iexternal/libconfig/lib
LIB	+= -Lexternal/libconfig -lconfig

# wiringPi
INC += -Iexternal/wiringPi/wiringPi
LIB += -Lexternal/wiringPi/wiringPi -lwiringPi

SRC	= \
	config.cpp \
	sensors/sensor_button.cpp \
	sensors/sensor_led.cpp \
	sensors/sensor_dht11.cpp \
	sensors/sensor_relay.cpp \
	sensors/sensor_pir.cpp \
	sensors/sensor_flame.cpp \
	sensors/sensor_servo.cpp \
	sensors/sensor_manager.cpp \
	client_main.cpp

OBJ=$(SRC:.cpp=.o)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: all clean

all: shp_client

shp_client: $(OBJ)
	$(CC) -o $@ $^ $(LIB)

clean:
	rm -f *.o sensors/*.o shp_client

config.o: config.hpp
sensors/sensor_button.o: sensors/sensor_button.hpp sensors/sensor.hpp
sensors/sensor_led.o: sensors/sensor_led.hpp sensors/sensor.hpp
sensors/sensor_dht11.o: sensors/sensor_dht11.hpp sensors/sensor.hpp
sensors/sensor_relay.o: sensors/sensor_relay.hpp sensors/sensor.hpp
sensors/sensor_pir.o: sensors/sensor_pir.hpp sensors/sensor.hpp
sensors/sensor_flame.o: sensors/sensor_flame.hpp sensors/sensor.hpp
sensors/sensor_servo.o: sensors/sensor_servo.hpp sensors/sensor.hpp

sensors/sensor_manager.o: sensors/sensor_manager.hpp sensors/sensor.hpp
client_main.o: config.hpp sensors/sensor_button.hpp sensors/sensor_led.hpp sensors/sensor_dht11.hpp sensors/sensor_relay.hpp sensors/sensor_pir.hpp sensors/sensor_manager.hpp
