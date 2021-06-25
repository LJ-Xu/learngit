#!/bin/bash
if [ ! -d "build" ];then
  mkdir build
fi
ISX32=1
if [ -n "`uname -a|grep 64`" ];then
  ISX32=0
fi


cd build
if [ $ISX32 -eq 0 ];then
. /opt/env-cross/env-a8-hmi.sh
cmake .. -DCMAKE_TOOLCHAIN_FILE=../arm_linux_gnueabihf.cmake 
make -j8 && arm-linux-gnueabihf-strip HmiMain 
[[ $? -ne 0 ]] && exit 
cp ./XJSocket.out/*.a ../Lib/armlinux/
cp ./DataClient.out/*.a ../Lib/armlinux/
cp ./DataService.out/*.a ../Lib/armlinux/
else
cmake .. -DUBUNTU=on -DLX32=1
make -j8
[[ $? -ne 0 ]] && exit 
cp ./XJSocket.out/*.a ../Lib/ubuntu/
cp ./DataClient.out/*.a ../Lib/ubuntu/
cp ./DataService.out/*.a ../Lib/ubuntu/
fi
cd -

