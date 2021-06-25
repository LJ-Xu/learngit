cmake_minimum_required (VERSION 3.0)
project(Hmi)  

#include_directories(${PROJECT_SOURCE_DIR}/FLTK/include)
#include_directories(${PROJECT_SOURCE_DIR}/HmiComm)
#include_directories(${PROJECT_SOURCE_DIR}/UIControl/Include)
#include_directories(${PROJECT_SOURCE_DIR}/Project/Include)
#include_directories(${PROJECT_SOURCE_DIR}/MsgQueue)
#include_directories(${PROJECT_SOURCE_DIR}/DataService)
#include_directories(${PROJECT_SOURCE_DIR}/Json)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
IF(LX32)
  add_definitions(-DARM_PLAT)
endif()


include_directories(${PROJECT_SOURCE_DIR}/HmiMain/getopt/include)

include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/)
include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/FLTK/include)
include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/FLTK/)
include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/FLTK/src)
include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/sqlite/include)
include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/json/)
include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/log4cpp/include)
include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/googlepinyin/include)
include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/googlepinyin/)
include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/restrpc/include)
#include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/boost_1_65_1/)
include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/msgpack-c/include)
include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/qrcode/include)
include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/qrcodeex/include)
include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/breakpad/include/breakpad)

include_directories(${PROJECT_SOURCE_DIR}/HmiPart/HmiUtility/include)
include_directories(${PROJECT_SOURCE_DIR}/HmiPart/HmiCore/include)
include_directories(${PROJECT_SOURCE_DIR}/HmiPart/HmiComm/include)
include_directories(${PROJECT_SOURCE_DIR}/HmiPart/UIControl/include)
include_directories(${PROJECT_SOURCE_DIR}/HmiPart/HmiUIManager/include)
include_directories(${PROJECT_SOURCE_DIR}/HmiPart/MsgQueue/include)
include_directories(${PROJECT_SOURCE_DIR}/HmiPart/Project/include)
include_directories(${PROJECT_SOURCE_DIR}/HmiPart/Service/HmiStorageService/include)
include_directories(${PROJECT_SOURCE_DIR}/HmiPart/Service/DataService/include)
include_directories(${PROJECT_SOURCE_DIR}/HmiPart/Service/XJSocket/include)
include_directories(${PROJECT_SOURCE_DIR}/HmiPart/Service/DataClient/include)
include_directories(${PROJECT_SOURCE_DIR}/ThirdPart/boost_1_65_1)

include_directories(${PROJECT_SOURCE_DIR}/OSLayer/System/include)
aux_source_directory(./HmiMain/src SRC_LIST)

#message(WARNNING "CMAKE_SYSTEM_PROCESSOR ====== "${CMAKE_SYSTEM_PROCESSOR})
if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
	link_directories(${PROJECT_SOURCE_DIR}/ThirdPart/FLTK/lib_arm)
	set (FLTK_LIBS ${FLTK_LIBS} fltk X11 fltk_images Xrender Xcursor Xfixes Xft Xinerama Xdmcp Xau Xext fontconfig xcb png16 z jpeg expat freetype uuid)
	link_directories(${PROJECT_SOURCE_DIR}/ThirdPart/breakpad/lib_arm)
	set (FLTK_LIBS ${FLTK_LIBS}  breakpad breakpad_client)
	link_directories(${PROJECT_SOURCE_DIR}/ThirdPart/qrcodeex/lib_arm)
	set (FLTK_LIBS ${FLTK_LIBS}  zint )
	link_directories(${PROJECT_SOURCE_DIR}/ThirdPart/google/protobuf/lib_arm)
	set (FLTK_LIBS ${FLTK_LIBS}  protobuf)

	link_directories(${PROJECT_SOURCE_DIR}/ThirdPart/boost_1_65_1/linux/arm32)
	set (FLTK_LIBS ${FLTK_LIBS}  boost_filesystem boost_system)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
	link_directories(${PROJECT_SOURCE_DIR}/ThirdPart/FLTK/lib_x86)
	set (FLTK_LIBS ${FLTK_LIBS} fltk X11 fltk_images)
	link_directories(${PROJECT_SOURCE_DIR}/ThirdPart/boost_1_65_1/linux/x86)
	set (FLTK_LIBS ${FLTK_LIBS}  boost_filesystem boost_system)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "i686")
	link_directories(${PROJECT_SOURCE_DIR}/ThirdPart/FLTK/lib_i686)
	set (FLTK_LIBS ${FLTK_LIBS} fltk X11 fltk_images )


	link_directories(${PROJECT_SOURCE_DIR}/ThirdPart/boost_1_65_1/linux/i686)
	set (FLTK_LIBS ${FLTK_LIBS}  boost_filesystem boost_system)

	link_directories(${PROJECT_SOURCE_DIR}/ThirdPart/qrcodeex/lib_i686)
	set (FLTK_LIBS ${FLTK_LIBS}  zint )

	link_directories(${PROJECT_SOURCE_DIR}/ThirdPart/breakpad/lib_i686)
	set (FLTK_LIBS ${FLTK_LIBS}  breakpad breakpad_client)
endif (CMAKE_SYSTEM_PROCESSOR MATCHES "arm")

if(UBUNTU)
set (FLTK_LIBS ${FLTK_LIBS} boost_filesystem boost_system protobuf dl breakpad_client)
set(LIBRARYDIR Lib/ubuntu)
set(LIBRARYDIR ${LIBRARYDIR} ThirdPart/linux_lib)
endif()

link_directories(
${LIBRARYDIR})

add_subdirectory(HmiPart/HmiCore)
set (EXTRA_LIBS ${EXTRA_LIBS} HmiCore)


add_subdirectory(HmiPart/UIControl)
set (EXTRA_LIBS ${EXTRA_LIBS} UIControl)

add_subdirectory(HmiPart/Project)
set (EXTRA_LIBS ${EXTRA_LIBS} Project)


add_subdirectory(HmiPart/HmiUIManager)
set (EXTRA_LIBS ${EXTRA_LIBS} HmiUIManager)



#add_subdirectory(HmiPart/Service)
#set (EXTRA_LIBS ${EXTRA_LIBS} Service)


add_subdirectory(HmiPart/Service/HmiStorageService)
set (EXTRA_LIBS ${EXTRA_LIBS} HmiStorageService)

#add_subdirectory(HmiPart/Service/DataService)
#set (EXTRA_LIBS ${EXTRA_LIBS} DataService)

#add_subdirectory(HmiPart/Service/DataClient)
#set (EXTRA_LIBS ${EXTRA_LIBS} DataClient)

#add_subdirectory(HmiPart/Service/XJSocket)
#set (EXTRA_LIBS ${EXTRA_LIBS} XJSocket)

if(EXISTS "${PROJECT_SOURCE_DIR}/../uimainlib")
add_subdirectory(${PROJECT_SOURCE_DIR}/HmiPart/Service/DataService DataService.out)
set (EXTRA_LIBS ${EXTRA_LIBS} DataService)

add_subdirectory(${PROJECT_SOURCE_DIR}/HmiPart/Service/DataClient DataClient.out)
set (EXTRA_LIBS ${EXTRA_LIBS} DataClient)

add_subdirectory(${PROJECT_SOURCE_DIR}/HmiPart/Service/XJSocket XJSocket.out)
set (EXTRA_LIBS ${EXTRA_LIBS} XJSocket)
else()
set (EXTRA_LIBS ${EXTRA_LIBS} DataService DataClient XJSocket)
  IF(LX32)
    link_directories(${PROJECT_SOURCE_DIR}/Lib/ubuntu)
  else()
    link_directories(${PROJECT_SOURCE_DIR}/Lib/armlinux)
  endif()
endif()
 

add_subdirectory(HmiPart/HmiComm)
set (EXTRA_LIBS ${EXTRA_LIBS} HmiComm)

add_subdirectory(HmiPart/HmiUtility)
set (EXTRA_LIBS ${EXTRA_LIBS} HmiUtility)

add_subdirectory(ThirdPart/log4cpp)

IF ( CMAKE_BUILD_TYPE MATCHES "Debug" )
	set (EXTRA_LIBS ${EXTRA_LIBS} log4cppD)
ELSE ( CMAKE_BUILD_TYPE MATCHES "Debug" )
	set (EXTRA_LIBS ${EXTRA_LIBS} log4cpp)
ENDIF ( CMAKE_BUILD_TYPE MATCHES "Debug" )

add_subdirectory(ThirdPart/sqlite)
set (EXTRA_LIBS ${EXTRA_LIBS} sqlite)


add_subdirectory(OSLayer/System)
set (EXTRA_LIBS ${EXTRA_LIBS} System)

add_subdirectory(ThirdPart/googlepinyin)
set (EXTRA_LIBS ${EXTRA_LIBS} googlepinyin)

#message(WARNNING "EXTRA_LIBS = "${EXTRA_LIBS})



add_subdirectory(HmiPart/MsgQueue)
set (EXTRA_LIBS ${EXTRA_LIBS} MsgQueue)




add_executable(HmiMain ${SRC_LIST})  

target_link_libraries(HmiMain ${EXTRA_LIBS} ${FLTK_LIBS} pthread png Xtst  )
#target_link_libraries(Hmi ${FLTK_LIBS} ${EXTRA_LIBS})


