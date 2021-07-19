#!/bin/bash
if [ ! -d "build" ];then
  mkdir build
fi
ISX32=1
if [ -n "`uname -a|grep 64`" ];then
  ISX32=0
fi

#git处理
cdir=`pwd`
branch=`head -1 .git/HEAD | tail -1 `
branch=${branch#ref: refs/heads/}
hash=`head -1 .git/refs/heads/$branch | tail -1`
gitusr=`git config user.name`
gitemail=`git config user.email`
gitusr="${gitusr}#${gitemail}"
date=$(date "+%Y%m%d")
time=$(date "+%H%M%S")
echo $branch
echo $hash
echo $gitusr
echo $date
echo $time

file=HmiPart/HmiComm/include/__ExtInfo.h
echo "#pragma once" > $file
echo "#define EXE_GIT_BRANCH  \"${branch}\"" >> $file
echo "#define EXE_GIT_HASH  \"${hash}\"" >> $file
echo "#define EXE_GIT_AUTHOR  \"${gitusr}\"" >> $file
echo "#define EXE_VER_DATE  \"${date}\"" >> $file
echo "#define EXE_VER_TIME  \"${time}\"" >> $file


cd build
if [ $ISX32 -eq 0 ];then
. /opt/env-cross/env-a8-hmi.sh
cmake .. -DCMAKE_TOOLCHAIN_FILE=../arm_linux_gnueabihf.cmake 
make -j8 && eu-strip HmiMain -f HmiMain.sym #arm-linux-gnueabihf-strip HmiMain 
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

