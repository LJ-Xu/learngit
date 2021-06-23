#aux_source_directory(. src_files)
#add_library(HmiCore STATIC ${src_files})
add_library(HmiCore STATIC 
	src/HmiCore.cpp
	src/ProjectParser.cpp

	)


