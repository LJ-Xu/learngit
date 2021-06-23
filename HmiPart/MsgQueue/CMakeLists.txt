#aux_source_directory(. src_files)
#add_library(MsgQueue STATIC ${src_files})
add_library(MsgQueue STATIC 
	src/Message.cpp
	src/MsgQueue.cpp
	src/ListQueue.cpp

	)
