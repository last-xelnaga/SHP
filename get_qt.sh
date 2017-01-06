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


# Fix the bug, that actully was fixed by https://codereview.qt-project.org/#/c/157240/3
# but somehow it still exists in the current latest release
prf_file=$qt_source/qtbase/mkspecs/features/qt_common.prf
sed -i -e 's/greaterThan(QT_GCC_MAJOR_VERSION, 4): QMAKE_CXXFLAGS_WARN_ON += -Wdate-time/#greaterThan(QT_GCC_MAJOR_VERSION, 4): QMAKE_CXXFLAGS_WARN_ON += -Wdate-time/g' $prf_file
sed -i -e 's/greaterThan(QT_GCC_MAJOR_VERSION, 5): QMAKE_CXXFLAGS_WARN_ON += -Wshift-overflow=2 -Wduplicated-cond/#greaterThan(QT_GCC_MAJOR_VERSION, 5): QMAKE_CXXFLAGS_WARN_ON += -Wshift-overflow=2 -Wduplicated-cond/g' $prf_file

./configure -verbose -continue -prefix $qt_result -device linux-rasp-pi-g++ -device-option CROSS_COMPILE=$tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf- -opensource -confirm-license -reduce-exports -release -no-opengl -no-dbus -make libs
make
make install
