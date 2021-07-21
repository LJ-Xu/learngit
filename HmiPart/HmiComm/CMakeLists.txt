#aux_source_directory(. src_files)
#aux_source_directory( ./cereal src_files)
#aux_source_directory( ./sparsehash src_files)
#aux_source_directory( ./googlepinyin src_files)
#add_library(HmiComm STATIC ${src_files})

add_library(HmiComm STATIC 
	src/Constant.cpp
	src/stdafx.cpp
	src/DeleteHelper.cpp
	src/StackArray.cpp
	src/Logger.cpp
	src/version.cpp
	src/RunEnv.cpp
	src/Semaphore.cpp


	)
