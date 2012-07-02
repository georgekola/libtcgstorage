/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface.c
///
/// transport interface mapper
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////
#ifndef _TCGS_INTERFACE_H
#define _TCGS_INTERFACE_H

#include "tcgs_types.h"

typedef enum
{
	INTERFACE_ERROR_GOOD,
	INTERFACE_ERROR_INVALID_SECURITY_PROTOCOL_ID_PARAMATER,
	INTERFACE_ERROR_INVALID_TRANSFER_LENGTH_PARAMETER_ON_IF_SEND,
	INTERFACE_ERROR_OTHER_INVALID_COMMAND_PARAMETER,
	INTERFACE_ERROR_SYNCHRONOUS_PROTOCOL_VIOLATION,
	DATA_PROTECTION_ERROR
} TCGS_TPerError_t;

typedef enum
{
	IF_SEND,
	IF_RECV
} TCGS_Command_t;

/*
 * \brief The abstracted command block of the interface commands
 *
 * This structure describes format of interface commands as defined in
 * TCG ARchitecture Core Specification (section 3.3. Interface Communications).
 * Further mapping to host interface to device is additionally required.
 */
typedef struct
{
	TCGS_Command_t command;   		//Either IF-SEND or IF-RECV
	uint8          protocolId;      //Between 0x01 and 0x06
	uint32         length;          //The amount of data to be transferred
	uint32         comId;           //The ComID to be used, for Protocol IDs 0x01, 0x02, 0x06
} TCGS_CommandBlock_t;

typedef enum
{
	INTERFACE_SCSI,
	INTERFACE_ATA,
	INTERFACE_NVM_EXPRESS,
	INTERFACE_VIRTUAL,
} TCGS_Interface_t;

void TCGS_SetInterface(TCGS_Interface_t interface);

typedef TCGS_TPerError_t (*TCGS_SendCommand_t) (
	    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
	    TCGS_TPerError_t *tperError, void *outputPayload);

typedef struct
{
	//TCGS_InitCommand_t *init
	TCGS_SendCommand_t send;
} TCGS_IntefaceFunctions_t;

/*****************************************************************************
 * \brief Map command to current interface and send it to TPer. Return response and status.
 *
 * @param[in]  inputCommandBlock      input command block
 * @param[in]  inputPayload           input payload. NULL if command has no data
 * @param[out] tperError              interface command error status
 * @param[out] outputPayload          output payload
 *
 * \return ERROR_SUCCESS if interface command is successfully mapped to current transport
 * sent to TPer and the last returned response (error status code and payload). Error code
 * ERROR_INTERFACE is returned otherwise
 *
 *****************************************************************************/
TCGS_TPerError_t TCGS_SendCommand(
    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
    TCGS_TPerError_t *tperError, void *outputPayload);

void   TCGS_SetParameter(char *name, uint32 value);
uint32 TCGS_GetParameter(char *name);

#endif //TCGS_INTERFACE_H
