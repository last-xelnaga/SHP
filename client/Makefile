CFLAGS=-Wall -O2 -fno-omit-frame-pointer
#CFLAGS=-Wall -ggdb 

%.o: %.cpp
	g++ -c -o $@ $< $(CFLAGS)

.PHONY: all clean

all: shp_client

shp_client: main.o
	g++ -o $@ $^

clean:
	rm -f *.o shp_client
