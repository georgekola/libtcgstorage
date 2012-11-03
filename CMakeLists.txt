cmake_minimum_required (VERSION 2.6) 
project (LIBTCGSTORAGE) 

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBTCGSTORAGE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBTCGSTORAGE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LIBTCGSTORAGE_BINARY_DIR}/bin)
 
add_subdirectory (src) 
add_subdirectory (test)
add_subdirectory (vtper)
add_subdirectory (tcgsparm)

#TARGET_LINK_LIBRARIES(tcgstorage)
