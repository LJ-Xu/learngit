#!/bin/sh
if [ ! -d "build" ];then
  mkdir build
fi
ISX32=0
if [ -n "`uname -a|grep 64`" ];then
  ISX32=1
fi


cd build
. /opt/env-cross/env-a8-hmi.sh
cmake .. -DCMAKE_TOOLCHAIN_FILE=../arm_linux_gnueabihf.cmake -DLX32=$ISX32
make -j8 && arm-linux-gnueabihf-strip HmiMain 
#&& cp HmiMain ../HmiMain
if [ $? -eq 0 ];then
  if [ -d "../../umlib" ];then
    cp ./XJSocket.out/*.a ../../umlib/
    cp ./DataClient.out/*.a ../../umlib/
    cp ./DataService.out/*.a ../../umlib/
  fi
fi	
cd -

