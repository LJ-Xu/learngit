#aux_source_directory(./src src_files)
##aux_source_directory(./DataClient/src src_files)
#aux_source_directory(./DataService/src src_files)
#aux_source_directory(./HmiStorageService/src src_files)
##aux_source_directory(./XJSocket/src src_files)

##list(REMOVE_ITEM src_files ./src/GlobalWindow.cpp)

#add_library(Service STATIC ${src_files})
add_library(HmiStorageService STATIC 
	src/StorageServiceImpl.cpp
	src/AlarmStorage.cpp
	src/BaseStorage.cpp
	src/SampleStorage.cpp
	src/OperatorStorage.cpp
	src/Record/XJRecord.cpp
	src/BaseStorageService.cpp
	src/OperatorStorageService.cpp
	src/AlarmStorageService.cpp
	src/SampleStorageService.cpp
	src/Record/XJRecord.cpp
	src/RecipeStorage.cpp
	src/RecipeStorageService.cpp
	src/FileSave.cpp

		)

