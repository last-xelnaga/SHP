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


# check the machine architecture
arch=$(uname --machine)
if [ $arch == "x86_64" ]; then
    info "you are using x86 ($arch) architecture"
    info "do you want to setup the cross compiler?"
    read -p "[y/n]" -n 1 -r
    echo    # new line
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo
        # get cross-compile tools
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
        echo "PREFIX = $tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-" > src/makefile.prefix
    else
        echo "PREFIX = " > src/makefile.prefix
    fi
fi

echo
info "process gtest ..."
cd src/external/gtest
make clean
make
cd $root

echo
info "process libconfig ..."
cd src/external/libconfig
make clean
make
cd $root

echo
info "process wiringPi ..."
cd src/external/wiringPi/wiringPi
make clean
make static
cd $root

echo
info "process zlib ..."
cd src/external/zlib
make clean
make static
cd zpipe
make clean
make all
cp zpipe $root/
cd $root

echo
info "process shp ..."
cd src
make clean
make
cd $root
