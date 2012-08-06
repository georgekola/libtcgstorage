/////////////////////////////////////////////////////////////////////////////
/// tcgs_builder.h
///
/// Builder for IF-SEND and IF-RECV commands   
///
/// (c) Artem Zankovich, 2012
//////////////////////////////////////////////////////////////////////////////

#ifndef _TCGS_BUILDER_H
#define _TCGS_BUILDER_H  

#include "tcgs_types.h"
#include "tcgs_stream.h"
#include "tcgs_interface.h"
  
typedef enum
{
	LEVEL0_DISCOVERY,
	PACKET
} TCGS_InterfaceCommand_t; 

/*****************************************************************************
 * \brief Encodes both parts of interface commands: (1) a command block and
 * (2) a data payload
 *
 * @param[in]  command      Code of command to encode
 * @param[in]  data         Data of the command to encode. NULL if command has no data
 * @param[out] commandBlock buffer for generated command block part of interface command
 * @param[out] payload      buffer for generated interface command payload
 *
 * \return ERROR_SUCCESS if interface command is successfully generated,
 * error code otherwise
 *
 *****************************************************************************/
TCGS_Error_t TCGS_PrepareInterfaceCommand(TCGS_InterfaceCommand_t command, uint8 *data,
		TCGS_CommandBlock_t *commandBlock, void *payload);

#endif //TCGS_BUILDER_H
