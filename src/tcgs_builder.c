/////////////////////////////////////////////////////////////////////////////
/// tcgs_builder.c
///
/// Builder for IF-SEND and IF-RECV commands   
///
/// (c) Artem Zankovich
//////////////////////////////////////////////////////////////////////////////

#include "tcgs_builder.h"
#include "tcgs_interface.h"

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
   TCGS_CommandBlock_t *commandBlock, void *payload)
{
	switch (command)
	{
	case LEVEL0_DISCOVERY:
		commandBlock->command    = IF_RECV;
		commandBlock->protocolId = 0x01;
		commandBlock->length     = 0x01;
		commandBlock->comId      = 0x01;		
		break;
	case PACKET:
		commandBlock->command    = IF_SEND;
		commandBlock->protocolId = 0x01;
		commandBlock->length     = 0x00;	//length is to be updated when payload is generated
		commandBlock->comId      = 0x01;    //TODO: get ComId from data (or parameter?)
		break;
	} //switch (command) 
	
	return ERROR_SUCCESS;
}
