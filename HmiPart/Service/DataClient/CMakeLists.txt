#aux_source_directory(./src src_files)
##aux_source_directory(./DataClient/src src_files)
#aux_source_directory(./DataService/src src_files)
#aux_source_directory(./HmiStorageService/src src_files)
##aux_source_directory(./XJSocket/src src_files)

##list(REMOVE_ITEM src_files ./src/GlobalWindow.cpp)

#add_library(Service STATIC ${src_files})
add_library(DataClient STATIC 
	src/DataClient.cpp
	src/ClientMain.cpp
	src/dal/ClientSession.cpp
	src/fifo/NolockFifo.cpp
	src/package/PackageMgr.cpp
	src/ClientServer.cpp
	src/CommandMgr.cpp

	)

