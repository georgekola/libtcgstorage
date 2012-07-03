set(CMAKE_C_FLAGS "-DUNIT_TESTING")

include_directories (${LIBTCGSTORAGE_SOURCE_DIR}/src ${LIBTCGSTORAGE_SOURCE_DIR}/vtper)

#link_directories (${LIBTCGSTORAGE_BINARY_DIR}/src) 

add_executable (testmain testmain.c tcgs_interface_virtual.c) 

target_link_libraries (testmain libtcgstorage vtper libcmockery.a) 
