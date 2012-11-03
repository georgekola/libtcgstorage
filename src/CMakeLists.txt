set(CMAKE_C_FLAGS "")



###################################################
# Source files
###################################################
file(GLOB lib_srcs "*.c")
source_group("Source" FILES ${lib_srcs})
IF(WIN32)
	file(GLOB lib_win_srcs "win/*.c")
	source_group("Source.win" FILES ${lib_win_srcs})
ENDIF(WIN32)
IF(UNIX)

ENDIF(UNIX)

###################################################
# Include files
###################################################
file(GLOB lib_hdrs "*.h")
source_group("Include" FILES ${lib_hdrs})
IF(WIN32)
	file(GLOB lib_hdrs "win/*.h")
	source_group("Include.win" FILES ${lib_win_hdrs})
ENDIF(WIN32)
IF(UNIX)
    
ENDIF(UNIX)

###################################################
# Target library
###################################################
IF(WIN32)
	add_library (tcgstorage ${lib_srcs} ${lib_win_srcs})
ENDIF(WIN32)
IF(UNIX)

ENDIF(UNIX)
