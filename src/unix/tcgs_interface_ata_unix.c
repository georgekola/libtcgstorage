/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface_ata.c
///
/// Implementation of ATA interface for UNIX
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#include "../tcgs_interface.h"
#include "../tcgs_interface_ata.h"
#include "../tcgs_types.h"

#include <stdio.h>

TCGS_IntefaceParameter_t parameter[] =
{
	{"ata.transport_mode", (uint32)ATA_TRANSPORT_DMA},
};

TCGS_IntefaceParameters_t parameters[] =
{
	sizeof(parameter) / sizeof(parameter[0]),
	parameter
};

#define BLOCK_SIZE 512
//TODO: fix handling of x=0
#define BYTES2BLOCKS(x) (((x - 1) >> 9) + 1)    // fit data to 512 blocks

static TCGS_Error_t TCGS_ATA_Open(char* device)
{
    printf("(not implemented)\n");
    return ERROR_INTERFACE;
}

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
static TCGS_Error_t TCGS_ATA_Send(
    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
    TCGS_InterfaceError_t *tperError, void *outputPayload)
{
    printf("(not implemented)\n");
    return ERROR_INTERFACE;
}

static void TCGS_ATA_SetParameter(char *name, uint32 value)
{
	return;
}

static uint32 TCGS_ATA_GetParameter(char *name)
{
	return 0;
}

TCGS_InterfaceDescriptor_t TCGS_ATA_InterfaceDescriptor =
{
	INTERFACE_ATA,
	(TCGS_OpenCommand_t)&TCGS_ATA_Open,
	(TCGS_IoCommand_t)&TCGS_ATA_Send,
	(TCGS_SetParameterCommand_t)&TCGS_ATA_SetParameter,
	(TCGS_GetParameterCommand_t)&TCGS_ATA_GetParameter,
};
