set(CMAKE_C_FLAGS "")

include_directories (${LIBTCGSTORAGE_SOURCE_DIR}/src 
${LIBTCGSTORAGE_SOURCE_DIR}/vtper ${LIBTCGSTORAGE_SOURCE_DIR}/tcgsparm)

###################################################
# Source files
###################################################
file(GLOB tcgsparm_srcs "*.c")
source_group("Source" FILES ${tcgsparm_srcs})
IF(WIN32)
	file(GLOB tcgsparm_win_srcs "win/*.c")
	source_group("Source.win" FILES ${tcgsparm_win_srcs})
ENDIF(WIN32)
IF(UNIX)
    
ENDIF(UNIX)

###################################################
# Include files
###################################################
file(GLOB tcgsparm_hdrs "*.h")
source_group("Include" FILES ${tcgsparm_hdrs})

###################################################
# Target executable
###################################################
IF(WIN32)
	add_executable (tcgsparm ${tcgsparm_srcs} ${tcgsparm_win_srcs})
ENDIF(WIN32)
IF(UNIX)
    
ENDIF(UNIX)

target_link_libraries (tcgsparm tcgstorage vtper)

