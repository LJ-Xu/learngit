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
cp ./XJSocket.out/*.a ../../umlib/OutputLib/armlinux/
cp ./DataClient.out/*.a ../../umlib/OutputLib/armlinux/
cp ./DataService.out/*.a ../../umlib/OutputLib/armlinux/
else
cmake .. -DUBUNTU=on -DLX32=1
make -j8
[[ $? -ne 0 ]] && exit 
cp ./XJSocket.out/*.a ../../umlib/OutputLib/ubuntu/
cp ./DataClient.out/*.a ../../umlib/OutputLib/ubuntu/
cp ./DataService.out/*.a ../../umlib/OutputLib/ubuntu/
fi
cd -

