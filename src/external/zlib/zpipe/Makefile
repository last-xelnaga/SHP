
# ./zpipe  < shp_client > shp_client.gz
# ./zpipe -d < shp_client.gz > shp_client

.PHONY: all clean

all: zpipe

zpipe :
	gcc zpipe.c -I.. -L.. -lz -o zpipe
	chmod +x zpipe

clean :
	rm zpipe
