#!/bin/sh
if [ ! -d "build" ];then
  mkdir build
fi
cd build
cmake .. -DUBUNTU=on
make -j8
#&& cp HmiMain ../HmiMain
cd -

