/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface.h
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
	INTERFACE_ERROR_DATA_PROTECTION,
} TCGS_InterfaceError_t;

typedef enum
{
	IF_SEND,
	IF_RECV,
	IF_LAST,	//special value that stores number of values in this enum
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
	uint32         length;          //The amount of data to be transferred, in bytes
	uint32         comId;           //The ComID to be used, for Protocol IDs 0x01, 0x02, 0x06
} TCGS_CommandBlock_t;

/*****************************************************************************
 * \brief Supported transport interfaces
 *
 * List of supported (now or in the future) transport interfaces
 * defined in SIIS specification
 *
 *****************************************************************************/
typedef enum
{
	INTERFACE_UNKNOWN,
	INTERFACE_SCSI,
	INTERFACE_ATA,
	INTERFACE_NVM_EXPRESS,
} TCGS_Interface_t;

/*****************************************************************************
 * \brief Switches set of interface functions
 *
 * @param[in]  interface              interface type
 *
 * \return None
 *
 *****************************************************************************/
void TCGS_SetInterface(TCGS_Interface_t interface);

typedef TCGS_InterfaceError_t (*TCGS_SendCommand_t) (
	    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
	    TCGS_InterfaceError_t *interfaceError, void *outputPayload);


/*****************************************************************************
 * \brief Current set of pointer to interface functions
 *
 * This structure contains pointers to transport-specific implementation of
 * transport functions. Don't use it directly, use TSGS_SetInterface function
 * instead
 *
 * \see TCGS_SetInterface
 *
 *****************************************************************************/
typedef struct
{
	//TCGS_InitCommand_t *init
	TCGS_SendCommand_t send;
} TCGS_InterfaceFunctions_t;

void TCGS_SetInterfaceFunctions(TCGS_InterfaceFunctions_t *functs);

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
TCGS_InterfaceError_t TCGS_SendCommand(
    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
    TCGS_InterfaceError_t *interfaceError, void *outputPayload);

/*****************************************************************************
 * \brief Set transport-dependent parameter
 *
 * It depends on implementation of concrete transport protocol how to use
 * provided parameter
 *
 * @param[in]  name                   name of the parameter
 * @param[in]  value                  value of the parameter
 *
 * \return None
 *
 * \see TSGS_GetInterfaceParameter
 *
 *****************************************************************************/
void   TCGS_SetInterfaceParameter(char *name, uint32 value);

/*****************************************************************************
 * \brief Get device-controlled transport-dependent parameter
 *
 * It depends on implementation of concrete transport protocol how to set
 * provided parameter. For example, ATA interface implementation may set
 * parameters that specify value of Trusted Feature Set support flag
 * form Identify Device response
 *
 * @param[in]  name                   name of the parameter
 *
 * \return     uint32                 value of the parameter
 *
 * \see TSGS_SetInterfaceParameter
 *
 *****************************************************************************/
uint32 TCGS_GetInterfaceParameter(char *name);

#endif //TCGS_INTERFACE_H
