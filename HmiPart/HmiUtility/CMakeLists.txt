#aux_source_directory(./src/Formate src_files)

#add_library(HmiUtility STATIC ${src_files})

add_library(HmiUtility STATIC 
	src/Formate/DataFormate.cpp

	)

