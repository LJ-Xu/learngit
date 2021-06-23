cmake_minimum_required(VERSION 3.1)

if (ARM)
	#SET(CMAKE_C_COMPILER   /mnt/c/code/rpm/gcc-linaro-7.4.1-2019.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
	#SET(CMAKE_CXX_COMPILER /mnt/c/code/rpm/gcc-linaro-7.4.1-2019.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)
endif()

project(example)
set(TARGET System)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11")


include_directories(
        "./include"
)

link_directories(
)

aux_source_directory(./src/ SOURCES)

message(STATUS "projectdir: " ${PROJECT_SOURCE_DIR})
message(STATUS "targetdir: " ${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE})

add_library(${TARGET} STATIC ${SOURCES})
#install(TARGETS ${TARGET} DESTINATION ${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE})
