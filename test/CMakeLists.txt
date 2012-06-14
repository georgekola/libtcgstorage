include_directories (${LIBTCGSTORAGE_SOURCE_DIR}/src) 

link_directories (${LIBTCGSTORAGE_BINARY_DIR}/src) 

add_executable (testmain testmain.c) 

target_link_libraries (testmain libtcgstorage)