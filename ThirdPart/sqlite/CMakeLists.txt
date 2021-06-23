aux_source_directory(./ src_files)
add_library(sqlite STATIC ${src_files})
target_link_libraries(sqlite dl)
