/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface_virtual.c
///
/// Virtual TPer interface mapper
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#include "tcgs_interface_vtper.h"
#include "tcgs_types.h"
#include "tcgs_interface.h"
#include "vtper.h"


static void TCGS_VTper_Init(void)
{
	return;
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
static TCGS_InterfaceError_t TCGS_VTper_Send(
    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
    TCGS_InterfaceError_t *tperError, void *outputPayload)
{
	return TCGS_VTPER_SendCommand(inputCommandBlock, inputPayload, tperError, outputPayload);
}

static void TCGS_VTper_SetParameter(char *name, uint32 value)
{
	return;
}

static uint32 TCGS_VTper_GetParameter(char *name)
{
	return 0;
}

TCGS_InterfaceDescriptor_t TCGS_VTper_InterfaceDescriptor =
{
	INTERFACE_VTPER,
	(TCGS_OpenCommand_t)&TCGS_VTper_Init,
	(TCGS_IoCommand_t)&TCGS_VTper_Send,
	(TCGS_SetParameterCommand_t)&TCGS_VTper_SetParameter,
	(TCGS_GetParameterCommand_t)&TCGS_VTper_GetParameter
};
