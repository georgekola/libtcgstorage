set(CMAKE_C_FLAGS "")

file(GLOB lib_srcs "*.c")
source_group("Source" FILES ${lib_srcs})

file(GLOB lib_hdrs "*.h")
source_group("Include" FILES ${lib_hdrs})

add_library (tcgstorage ${lib_srcs})
