INC=-I.
CFLAGS=-Wall -O2 -fno-omit-frame-pointer
LIB=-L../external/libconfig -lconfig -L../external/wiringPi -lwiringPi -lpthread
#CFLAGS+=-Wall -ggdb 

SRC=main.cpp

OBJ=$(SRC:.cpp=.o)

%.o: %.cpp
	g++ $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: all clean

all: shp_client

shp_client: $(OBJ)
	g++ -o $@ $^ $(LIB)

clean:
	rm -f *.o shp_client
