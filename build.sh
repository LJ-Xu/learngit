#!/bin/sh
if [ ! -d "build" ];then
  mkdir build
fi
cd build
. /opt/env-cross/env-a8-hmi.sh
cmake .. -DCMAKE_TOOLCHAIN_FILE=../arm_linux_gnueabihf.cmake
make -j8 && arm-linux-gnueabihf-strip HmiMain 
#&& cp HmiMain ../HmiMain
cd -

