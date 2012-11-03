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
	file(GLOB lib_unix_srcs "unix/*.c")
	source_group("Source.unix" FILES ${lib_unix_srcs})
ENDIF(UNIX)

###################################################
# Include files
###################################################
file(GLOB lib_hdrs "*.h")
source_group("Include" FILES ${lib_hdrs})
IF(WIN32)
	file(GLOB lib_win_hdrs "win/*.h")
	source_group("Include.win" FILES ${lib_win_hdrs})
ENDIF(WIN32)
IF(UNIX)
	file(GLOB lib_unix_hdrs "unix/*.h")
	source_group("Include.unix" FILES ${lib_unix_hdrs})
ENDIF(UNIX)

###################################################
# Target library
###################################################
IF(WIN32)
	add_library (tcgstorage ${lib_srcs} ${lib_win_srcs})
ENDIF(WIN32)
IF(UNIX)
	add_library (tcgstorage ${lib_srcs} ${lib_unix_srcs})
ENDIF(UNIX)
