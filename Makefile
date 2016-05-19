
.PHONY: all clean

all: shp_client shp_proxy shp_test

shp_client:
	make -f makefile.client

shp_proxy:
	make -f makefile.proxy

shp_test:
	make -f makefile.test

clean:
	make -f makefile.client clean
	make -f makefile.proxy clean
	make -f makefile.test clean
