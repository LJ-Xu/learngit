#aux_source_directory(./src src_files)
##aux_source_directory(./DataClient/src src_files)
#aux_source_directory(./DataService/src src_files)
#aux_source_directory(./HmiStorageService/src src_files)
##aux_source_directory(./XJSocket/src src_files)

##list(REMOVE_ITEM src_files ./src/GlobalWindow.cpp)

#add_library(Service STATIC ${src_files})

add_library(XJSocket STATIC 
	src/LinuxSocketClient.cpp  
	src/LinuxSocketClientMulti.cpp  
	src/LinuxSocketServer.cpp  
	src/Main.cpp 
	src/WinSocketClient.cpp  
	src/WinSocketClientMulti.cpp  
	src/WinSocketServer.cpp  
	src/XJSocketClient.cpp  
	src/XJSocketClientMulti.cpp  
	src/XJSocketServer.cpp	
	)

