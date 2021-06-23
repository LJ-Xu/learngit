#aux_source_directory(./src src_files)
##aux_source_directory(./DataClient/src src_files)
#aux_source_directory(./DataService/src src_files)
#aux_source_directory(./HmiStorageService/src src_files)
##aux_source_directory(./XJSocket/src src_files)

##list(REMOVE_ITEM src_files ./src/GlobalWindow.cpp)

#add_library(Service STATIC ${src_files})
add_library(DataService  STATIC 
	src/DataService.cpp
	src/DataApi.cpp
	src/DataFrame.cpp
	src/LocalSysDataCache.cpp
	src/DataCache.cpp
	src/sysdatarequest.cpp
	src/hmidataserver.pb.cc

		)

