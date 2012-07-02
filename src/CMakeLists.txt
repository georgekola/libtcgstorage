set(CMAKE_C_FLAGS "")
add_library (libtcgstorage libtcgstorage.c tcgs_builder.c tcgs_interface_ata.c tcgs_interface_virtual.c tcgs_interface.c)
