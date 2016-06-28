#!/bin/bash

function info()
{
	echo -e "\e[00;32m$*\e[00m" >&2 #green
}

function warn()
{
	echo -e "\e[00;35m$*\e[00m" >&2 #purple
}

function error()
{
	echo -e "\e[00;31m$*\e[00m" >&2 #green
	exit 1
}

# workspace setup
root=$(pwd)
tools=$root/../tools
target=""

if [ -z "$1" ]; then
	echo "no argument supplied, so build for arm"
else
	echo "argument supplied, so build for pc"
	target="pc"
fi

# get cross-compile tools
if [ -z "$target" ]; then
	if [ ! -d $tools ]; then
		cd $root/..

		info "clone tools..."
		git clone https://github.com/raspberrypi/tools
		if [ "$?" -ne "0" ]; then
			error "failed to get tools"
		fi

		info "checkout old but stable version..."
		cd $tools
		git checkout 3a413ca2b23fd275e8ddcc34f3f9fc3a4dbc723f
		if [ "$?" -ne "0" ]; then
			error "failed to checkout"
		fi
	else
		info "tools already in place, skipping..."
	fi

	info "create makefile.prefix ..."
	cd $root
	#if [ ! -f makefile.prefix ]; then
	 	echo "PREFIX	= $tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-" > makefile.prefix
	#fi
else
	echo "PREFIX	= " > makefile.prefix
fi


info "process gtest ..."
cd external/gtest
make clean
make
cd $root

info "process libconfig ..."
cd external/libconfig
make clean
make
cd $root

info "process wiringPi ..."
cd external/wiringPi/wiringPi
make clean
make static
cd $root

info "process shp ..."
make clean
make
