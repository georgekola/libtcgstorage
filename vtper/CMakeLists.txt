set(CMAKE_C_FLAGS "")

include_directories (${LIBTCGSTORAGE_SOURCE_DIR}/src ${LIBTCGSTORAGE_SOURCE_DIR}/vtper)

file(GLOB lib_srcs "*.c")
source_group("Source" FILES ${lib_srcs})

file(GLOB lib_hdrs "*.h")
source_group("Include" FILES ${lib_hdrs})

add_library (vtper ${lib_srcs})

