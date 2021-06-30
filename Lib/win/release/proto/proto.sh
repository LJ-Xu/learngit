#!/bin/bash

in_dir="."
out_dir="./src/"

./protoc --proto_path=$in_dir --cpp_out=$out_dir \
"esunnyrisk_base.proto" \

