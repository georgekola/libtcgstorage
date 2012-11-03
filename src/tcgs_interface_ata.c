/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface_ata.c
///
/// ATA interface mapper
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#include "tcgs_interface.h"
#include "tcgs_interface_ata.h"
#include "tcgs_types.h"


/*****************************************************************************
 * \brief Map command to ATA interface and send it to TPer. Return response and status.
 *
 * @param[in]  inputCommandBlock      input command block
 * @param[in]  inputPayload           input payload. NULL if command has no data
 * @param[out] tperError              interface command error status
 * @param[out] outputPayload          output payload
 *
 * \return ERROR_SUCCESS if interface command is successfully mapped to ATA transport
 * sent to TPer and the last returned response (error status code and payload). Error code
 * ERROR_INTERFACE is returned otherwise
 *
 *****************************************************************************/
static TCGS_InterfaceError_t TCGS_ATA_Send(
    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
    TCGS_InterfaceError_t *tperError, void *outputPayload)
{
	*tperError = INTERFACE_ERROR_GOOD;
	return ERROR_SUCCESS;
}

static void TCGS_ATA_SetParameter(char *name, uint32 value)
{
	return;
}

static uint32 TCGS_ATA_GetParameter(char *name)
{
	return 0;
}

/*
TCGS_InterfaceDescriptor_t TCGS_ATA_InterfaceDescriptor =
{
	INTERFACE_ATA,
	(TCGS_OpenCommand_t)&TCGS_ATA_Open,
	(TCGS_IoCommand_t)&TCGS_ATA_Send,
	(TCGS_SetParameterCommand_t)&TCGS_ATA_SetParameter,
	(TCGS_GetParameterCommand_t)&TCGS_ATA_GetParameter,
};
*/

