set(CMAKE_C_FLAGS "")
add_library (libtcgstorage libtcgstorage.c tcgs_builder.c tcgs_parser.c tcgs_interface_ata.c tcgs_interface.c tcgs_interface_encode.c)
