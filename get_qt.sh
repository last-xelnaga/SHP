#!/bin/bash -x

# workspace setup
root=$(pwd)
tools=$root/../tools
qt_source=$root/../qt5
qt_result=$root/../qt


if [ ! -d $qt_source ]; then
    cd $root/..
    git clone https://code.qt.io/qt/qt5.git
    cd $qt_source
    ./init-repository
else
    cd $qt_source
    ./init-repository -f
fi

./configure -verbose -continue -prefix $qt_result -device linux-rasp-pi-g++ -device-option CROSS_COMPILE=$tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf- -opensource -confirm-license -reduce-exports -release -no-opengl -no-dbus -make libs
make
#make install
