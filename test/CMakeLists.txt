set(CMAKE_C_FLAGS "-DUNIT_TESTING")

include_directories (${LIBTCGSTORAGE_SOURCE_DIR}/src ${LIBTCGSTORAGE_SOURCE_DIR}/vtper)

file(GLOB test_srcs "*.c")
source_group("Source" FILES ${test_srcs})

file(GLOB lib_hdrs "*.h")
source_group("Include" FILES ${test_hdrs})

add_executable (testmain ${test_srcs})

target_link_libraries (testmain tcgstorage vtper libcmockery.a) 
