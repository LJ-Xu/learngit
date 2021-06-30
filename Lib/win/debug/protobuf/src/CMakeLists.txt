cmake_minimum_required(VERSION 3.1)

if (ARM)
	SET(CMAKE_C_COMPILER   /mnt/c/code/rpm/gcc-linaro-7.4.1-2019.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
	SET(CMAKE_CXX_COMPILER /mnt/c/code/rpm/gcc-linaro-7.4.1-2019.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)
endif()

project(example)
set(TARGET protomsg)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE -g -pthread -std=c++11")


#SET(Boost_INCLUDE_DIR ../Library)
#linux library dir
#SET(BOOST_LIBRARYDIR /mnt/hgfs/code/dataserver/LIB/ubuntu)
#SET(BOOST_LIBRARYDIR ../LIB/linux)
#arm32 library dir
#SET(BOOST_LIBRARYDIR /mnt/c/code/3rd/boost_1_65_1/arm32/release)

#find_package(Boost COMPONENTS system filesystem REQUIRED)
#find_package(JNI REQUIRED)

include_directories(
	"."
)

if (ARM)
	set(LIBRARYDIR ../LIB/arm32)
elseif(UBUNTU)
	set(LIBRARYDIR ../LIB/ubuntu)
else()
	set(LIBRARYDIR ../LIB/linux)
endif()

message(STATUS "librarydir: " ${LIBRARYDIR})
link_directories(
	${LIBRARYDIR}
#	"../Library/oslayer/build"
#	"../Library/XJSocket/build"
#	"../Library/threadpool/build"
#	"../Modules/HmiComm/build"
#	"../Modules/Project/build"
#	"../Modules/ConnDevData/build"
#	"../Modules/LocalData/build"
#	"../Modules/DataRepository/build"
#	"../Modules/UserMacro/build"
#	"../Modules/DataServer/build"
)
add_subdirectory(../Library/oslayer oslayer_build)
add_subdirectory(../Library/XJSocket XJSocket_build)
add_subdirectory(../Library/threadpool threadpool_build)
add_subdirectory(../Modules/HmiComm HmiComm_build)
add_subdirectory(../Modules/Project Project_build)
add_subdirectory(../Modules/ConnDevData ConnDevData_build)
add_subdirectory(../Modules/LocalData LocalData_build)
add_subdirectory(../Modules/DataRepository DataRepository_build)
add_subdirectory(../Modules/UserMacro UserMacro_build)
add_subdirectory(../Modules/DataServer DataServer_build)

aux_source_directory(./src/ SOURCES)

message(STATUS "projectdir: " ${PROJECT_SOURCE_DIR})
message(STATUS "targetdir: " ${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE})

add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} DataServer DataRepository ConnDevData LocalData UserMacro Project HmiComm System ThreadPool XJSocket log4cpp dl breakpad_client boost_filesystem boost_system)
#target_link_libraries(${TARGET} DataServer DataRepository ConnDevData LocalData UserMacro Project HmiComm System ThreadPool XJSocket log4cpp dl X11 boost_filesystem boost_system)
#target_link_libraries(${TARGET} DataServer DataRepository ConnDevData LocalData UserMacro Project HmiComm System ThreadPool XJSocket log4cpp dl X11 ${Boost_LIBRARIES})



