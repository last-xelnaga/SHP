
.PHONY: all clean

all: shp_client shp_proxy shp_test

shp_client:
	make -f client.makefile

shp_proxy:
	make -f proxy.makefile

shp_test:
	make -f makefile.test

clean:
	make -f client.makefile clean
	make -f proxy.makefile clean
	make -f makefile.test clean

# valgrind --leak-check=full --show-leak-kinds=all ./shp_client
