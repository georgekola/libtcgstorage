/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface_vtper.c
///
/// Virtual TPer interface mapper
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "tcgs_interface_vtper.h"
#include "tcgs_types.h"
#include "tcgs_interface.h"
#include "vtper.h"


static TCGS_Error_t TCGS_VTper_Init(void)
{
	return ERROR_SUCCESS;
}

static TCGS_Error_t TCGS_VTper_Close(void)
{
	return ERROR_SUCCESS;
}

/*****************************************************************************
 * \brief Map command to virtual TPer interface and send it to TPer.
 *        Return command response buffer and interface status
 *
 * @param[in]  inputCommandBlock      input command block
 * @param[in]  inputPayload           input payload. NULL if command has no data
 * @param[out] tperError              interface command error status
 * @param[out] outputPayload          output payload
 *
 * \return ERROR_SUCCESS if interface command is successfully mapped to Virtual
 * TPer transport sent to TPer and the last returned response (error status
 * code and payload). Error code ERROR_INTERFACE is returned otherwise
 *
 *****************************************************************************/
static TCGS_InterfaceError_t TCGS_VTper_IoCommand(
    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
    TCGS_InterfaceError_t *tperError, void *outputPayload)
{
	return TCGS_VTPER_IoCommand(inputCommandBlock, inputPayload, tperError, outputPayload);
}

TCGS_InterfaceParameters_t* TCGS_VTper_GetParameters (void)
{
    return NULL;
}

TCGS_Error_t TCGS_VTper_SetDeviceParameter (char *name, uint32 value)
{
    return ERROR_INTERFACE;
}

TCGS_Error_t TCGS_VTper_UpdateDeviceParameters (void)
{
    return ERROR_INTERFACE;
}

TCGS_InterfaceDescriptor_t TCGS_VTper_InterfaceDescriptor =
{
	INTERFACE_VTPER,
	(TCGS_OpenCommand_t)&TCGS_VTper_Init,
	(TCGS_CloseCommand_t)&TCGS_VTper_Close,
	(TCGS_IoCommand_t)&TCGS_VTper_IoCommand,
	(TCGS_GetParameters_t)&TCGS_VTper_GetParameters,
	(TCGS_SetDeviceParameter_t)&TCGS_VTper_SetDeviceParameter,
	(TCGS_UpdateDeviceParameters_t)&TCGS_VTper_UpdateDeviceParameters,
};
